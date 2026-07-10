const mysql = require('mysql2/promise');
require('dotenv').config();

async function seedDatabase() {
    console.log("Iniciando el script de llenado de base de datos...");
    
    let connection;
    try {
        connection = await mysql.createConnection({
            host: process.env.MYSQL_HOST,
            port: process.env.MYSQL_PORT,
            user: process.env.MYSQL_USER,
            password: process.env.MYSQL_PASSWORD,
            database: process.env.MYSQL_DATABASE,
            ssl: { rejectUnauthorized: false }
        });

        // 1. Eliminar tablas antiguas
        await connection.query('DROP TABLE IF EXISTS lectura_humedad');
        await connection.query('DROP TABLE IF EXISTS sensor');
        await connection.query('DROP TABLE IF EXISTS sector');
        await connection.query('DROP TABLE IF EXISTS usuario');

        // 2. Crear tablas
        await connection.query(`
            CREATE TABLE usuario (
                id_usuario INT AUTO_INCREMENT PRIMARY KEY,
                nombre VARCHAR(100),
                email VARCHAR(100)
            )
        `);
        await connection.query(`
            CREATE TABLE sector (
                id_sector INT AUTO_INCREMENT PRIMARY KEY,
                nombre_sector VARCHAR(100),
                tipo_cultivo VARCHAR(100),
                id_usuario INT,
                FOREIGN KEY (id_usuario) REFERENCES usuario(id_usuario)
            )
        `);
        await connection.query(`
            CREATE TABLE sensor (
                id_sensor INT AUTO_INCREMENT PRIMARY KEY,
                modelo VARCHAR(100),
                estado ENUM('activo', 'inactivo', 'mantenimiento'),
                id_sector INT,
                FOREIGN KEY (id_sector) REFERENCES sector(id_sector)
            )
        `);
        await connection.query(`
            CREATE TABLE lectura_humedad (
                id_lectura INT AUTO_INCREMENT PRIMARY KEY,
                id_sensor INT,
                nivel_humedad DECIMAL(5,2),
                temperatura DECIMAL(5,2),
                fecha_registro DATETIME,
                FOREIGN KEY (id_sensor) REFERENCES sensor(id_sensor)
            )
        `);

        // 3. Insertar Usuario
        await connection.query('INSERT INTO usuario (nombre, email) VALUES ?', [[['Admin', 'admin@mipoc.cl']]]);
        
        // 4. Insertar 3 Sectores donde se hicieron pruebas
        const sectores = [
            ['Jardín Frontal', 'Pasto y Arbustos', 1],
            ['Jardín Trasero', 'Flores', 1],
            ['Huerto', 'Hortalizas', 1]
        ];
        await connection.query('INSERT INTO sector (nombre_sector, tipo_cultivo, id_usuario) VALUES ?', [sectores]);

        // 5. Insertar 1 SOLO Sensor (Portátil, actualmente ubicado en el Huerto)
        const sensores = [
            ['Sensor-Humedad-Portátil', 'activo', 3]
        ];
        await connection.query('INSERT INTO sensor (modelo, estado, id_sector) VALUES ?', [sensores]);

        // 6. Insertar Lecturas (Todas asociadas al único sensor)
        const lecturas = [];
        for(let i=1; i<=300; i++) {
            const id_sensor = 1; 
            
            // Humedad variable para simular los distintos terrenos y forzar alertas críticas
            const humedad = (Math.random() * (75 - 15) + 15).toFixed(2);
            const temperatura = (Math.random() * (32 - 10) + 10).toFixed(2);
            
            const diasAtras = Math.floor(Math.random() * 7);
            const hora = Math.floor(Math.random() * (22 - 8 + 1)) + 8; 
            const minuto = Math.floor(Math.random() * 60);
            const segundo = Math.floor(Math.random() * 60);

            const fechaRandom = new Date();
            fechaRandom.setDate(fechaRandom.getDate() - diasAtras);
            fechaRandom.setHours(hora, minuto, segundo, 0);

            const year = fechaRandom.getFullYear();
            const month = String(fechaRandom.getMonth() + 1).padStart(2, '0');
            const day = String(fechaRandom.getDate()).padStart(2, '0');
            const hours = String(fechaRandom.getHours()).padStart(2, '0');
            const minutes = String(fechaRandom.getMinutes()).padStart(2, '0');
            const seconds = String(fechaRandom.getSeconds()).padStart(2, '0');

            const fechaFormat = `${year}-${month}-${day} ${hours}:${minutes}:${seconds}`;

            lecturas.push([id_sensor, humedad, temperatura, fechaFormat]);
        }
        
        await connection.query('INSERT INTO lectura_humedad (id_sensor, nivel_humedad, temperatura, fecha_registro) VALUES ?', [lecturas]);

        console.log("¡Éxito! Se inyectaron 3 jardines, 1 sensor portátil y 300 lecturas evaluables.");
    } catch (error) {
        console.error("Error en el proceso:", error);
    } finally {
        if (connection) await connection.end();
    }
}

seedDatabase();