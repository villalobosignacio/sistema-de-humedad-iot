const express = require('express');
const mysql = require('mysql2/promise');
const { MongoClient } = require('mongodb');
const cors = require('cors');
require('dotenv').config();

const app = express();
const PORT = process.env.PORT || 3000;

app.use(cors());
app.use(express.json());
app.use(express.static('public'));

// Pool de MySQL
const pool = mysql.createPool({
    host: process.env.MYSQL_HOST,
    port: process.env.MYSQL_PORT,
    user: process.env.MYSQL_USER,
    password: process.env.MYSQL_PASSWORD,
    database: process.env.MYSQL_DATABASE,
    ssl: { rejectUnauthorized: false }
});

// Cliente MongoDB
const mongoClient = new MongoClient(process.env.MONGODB_URI);
let mongoCollection;

async function connectMongo() {
    try {
        await mongoClient.connect();
        const db = mongoClient.db('iot_logs');
        mongoCollection = db.collection('historial_consultas');
        console.log("Conectado a MongoDB Atlas");
    } catch (err) {
        console.error("Error conectando a MongoDB:", err);
    }
}
connectMongo();

// Consultas predefinidas

const consultasPredefinidas = [
    { id: 1, nombre: "Promedio de Humedad por Sector", sql: "SELECT se.nombre_sector, AVG(l.nivel_humedad) as promedio_humedad FROM sector se JOIN sensor s ON se.id_sector = s.id_sector JOIN lectura_humedad l ON s.id_sensor = l.id_sensor GROUP BY se.id_sector" },
    { id: 2, nombre: "Lecturas Críticas (< 30% Humedad)", sql: "SELECT DATE_FORMAT(l.fecha_registro, '%Y-%m-%d %H:%i') as Fecha, s.modelo as Sensor, l.nivel_humedad as Humedad_Critica FROM lectura_humedad l JOIN sensor s ON l.id_sensor = s.id_sensor WHERE l.nivel_humedad < 30 ORDER BY l.fecha_registro DESC LIMIT 20" },
    { id: 3, nombre: "Total de Lecturas por Día", sql: "SELECT DATE(fecha_registro) as Dia_Registrado, COUNT(*) as Total_Lecturas FROM lectura_humedad GROUP BY DATE(fecha_registro) ORDER BY Dia_Registrado DESC" },
    { id: 4, nombre: "Estado actual de Sensores", sql: "SELECT estado as Estado_Sensor, COUNT(*) as Cantidad FROM sensor GROUP BY estado" },
    { id: 5, nombre: "Últimos Registros del Sensor (Historial)", sql: "SELECT s.modelo as Sensor, l.nivel_humedad as Porcentaje_Humedad, l.temperatura as Temperatura, DATE_FORMAT(l.fecha_registro, '%Y-%m-%d %H:%i') as Fecha_y_Hora FROM lectura_humedad l JOIN sensor s ON l.id_sensor = s.id_sensor ORDER BY l.fecha_registro DESC LIMIT 50" }
];

// Función para guardar historial en MongoDB
async function logQuery(tipo, consulta, resultadosCount, errorMsg = null) {
    if (!mongoCollection) return;
    try {
        await mongoCollection.insertOne({
            tipo,
            consulta,
            resultados_obtenidos: resultadosCount,
            error: errorMsg,
            fecha: new Date(),
            usuario_sistema: "admin"
        });
    } catch (err) {
        console.error("Error guardando log en Mongo:", err);
    }
}

// Endpoints
app.get('/api/status', async (req, res) => {
    let mysqlStatus = "Desconectado";
    let mongoStatus = mongoCollection ? "Conectado" : "Desconectado";
    try {
        const [rows] = await pool.query('SELECT 1');
        if(rows) mysqlStatus = "Conectado";
    } catch (err) {}
    res.json({ mysql: mysqlStatus, mongo: mongoStatus });
});

app.get('/api/consultas', (req, res) => {
    res.json(consultasPredefinidas);
});

app.get('/api/consultas/:id', async (req, res) => {
    const queryObj = consultasPredefinidas.find(c => c.id == req.params.id);
    if (!queryObj) return res.status(404).json({ error: "Consulta no encontrada" });
    
    try {
        const [rows] = await pool.query(queryObj.sql);
        logQuery('predefinida', queryObj.sql, rows.length);
        res.json(rows);
    } catch (err) {
        logQuery('predefinida', queryObj.sql, 0, err.message);
        res.status(500).json({ error: err.message });
    }
});

app.post('/api/sql', async (req, res) => {
    const { sql } = req.body;
    const isSelect = /^\s*(select|show|describe)/i.test(sql);
    if (!isSelect) {
        logQuery('libre', sql, 0, "Intento de escritura/borrado rechazado");
        return res.status(403).json({ error: "Solo se permiten consultas SELECT, SHOW o DESCRIBE." });
    }
    try {
        const [rows] = await pool.query(sql);
        logQuery('libre', sql, rows.length);
        res.json(rows);
    } catch (err) {
        logQuery('libre', sql, 0, err.message);
        res.status(500).json({ error: err.message });
    }
});

app.get('/api/historial', async (req, res) => {
    if (!mongoCollection) return res.json([]);
    try {
        const logs = await mongoCollection.find().sort({ fecha: -1 }).limit(50).toArray();
        res.json(logs);
    } catch (err) {
        res.status(500).json({ error: err.message });
    }
});
const path = require('path');

app.get('/', (req, res) => {
    res.sendFile(path.join(__dirname, 'public', 'index.html'));
});
app.listen(PORT, () => {
    console.log(`Servidor corriendo en el puerto ${PORT}`);
});