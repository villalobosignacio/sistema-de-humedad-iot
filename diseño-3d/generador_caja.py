def mm(v):
    return v / 10.0

def add_box(comp, name, x, y, z, lx, ly, lz):
    sk = comp.sketches.add(comp.xYConstructionPlane)
    sk.sketchCurves.sketchLines.addTwoPointRectangle(
        adsk.core.Point3D.create(mm(x), mm(y), 0),
        adsk.core.Point3D.create(mm(x + lx), mm(y + ly), 0)
    )
    prof = sk.profiles.item(0)
    ext = comp.features.extrudeFeatures
    inp = ext.createInput(prof, adsk.fusion.FeatureOperations.NewBodyFeatureOperation)
    inp.setDistanceExtent(False, adsk.core.ValueInput.createByReal(mm(lz)))
    feat = ext.add(inp)
    body = feat.bodies.item(0)
    body.name = name

    if z != 0:
        move_feats = comp.features.moveFeatures
        objs = adsk.core.ObjectCollection.create()
        objs.add(body)
        mat = adsk.core.Matrix3D.create()
        mat.translation = adsk.core.Vector3D.create(0, 0, mm(z))
        move_input = move_feats.createInput(objs, mat)
        move_feats.add(move_input)

    return body

def add_cylinder(comp, name, x, y, z, diameter, height):
    sk = comp.sketches.add(comp.xYConstructionPlane)
    sk.sketchCurves.sketchCircles.addByCenterRadius(
        adsk.core.Point3D.create(mm(x), mm(y), 0),
        mm(diameter / 2)
    )
    prof = sk.profiles.item(0)
    ext = comp.features.extrudeFeatures
    inp = ext.createInput(prof, adsk.fusion.FeatureOperations.NewBodyFeatureOperation)
    inp.setDistanceExtent(False, adsk.core.ValueInput.createByReal(mm(height)))
    feat = ext.add(inp)
    body = feat.bodies.item(0)
    body.name = name

    if z != 0:
        move_feats = comp.features.moveFeatures
        objs = adsk.core.ObjectCollection.create()
        objs.add(body)
        mat = adsk.core.Matrix3D.create()
        mat.translation = adsk.core.Vector3D.create(0, 0, mm(z))
        move_input = move_feats.createInput(objs, mat)
        move_feats.add(move_input)

    return body

def cut_round_hole(comp, x, y, diameter, depth):
    sk = comp.sketches.add(comp.xYConstructionPlane)
    sk.sketchCurves.sketchCircles.addByCenterRadius(
        adsk.core.Point3D.create(mm(x), mm(y), 0),
        mm(diameter / 2)
    )
    prof = sk.profiles.item(0)
    ext = comp.features.extrudeFeatures
    inp = ext.createInput(prof, adsk.fusion.FeatureOperations.CutFeatureOperation)
    inp.setDistanceExtent(False, adsk.core.ValueInput.createByReal(mm(depth)))
    ext.add(inp)

def run(context):
    ui = None
    try:
        app = adsk.core.Application.get()
        ui = app.userInterface
        design = app.activeProduct
        comp = design.rootComponent

        L = 150
        W = 80
        H = 35

        wall = 3
        floor = 3

        prong_w = 10
        gap_w = 2
        sensor_h = 2
        total_sensor_w = prong_w * 2 + gap_w
        sensor_y = (W - total_sensor_w) / 2

        # =========================
        # CAJA ABIERTA CON SUELO LISO
        # =========================

        add_box(comp, "Piso liso caja", 0, 0, 0, L, W, floor)

        # Paredes largas
        add_box(comp, "Pared lateral 1", 0, 0, floor, L, wall, H - floor)
        add_box(comp, "Pared lateral 2", 0, W - wall, floor, L, wall, H - floor)

        # Pared izquierda CERRADA, sin USB
        add_box(comp, "Pared frontal cerrada", 0, 0, floor, wall, W, H - floor)

        # Sensor abajo, extremo derecho, con separador central
        x_sensor = L - wall

        add_box(comp, "Pared sensor izquierda", x_sensor, 0, floor, wall, sensor_y, H - floor)
        add_box(comp, "Separador sensor central", x_sensor, sensor_y + prong_w, floor, wall, gap_w, sensor_h)
        add_box(comp, "Pared sensor derecha", x_sensor, sensor_y + total_sensor_w, floor, wall, W - sensor_y - total_sensor_w, H - floor)
        add_box(comp, "Puente superior sensor", x_sensor, sensor_y, floor + sensor_h, wall, total_sensor_w, H - floor - sensor_h)

        # =========================
        # POSTES Y AGUJEROS DE TAPA
        # =========================
        tapa_holes = [
            (7, 7),
            (L - 7, 7),
            (7, W - 7),
            (L - 7, W - 7)
        ]

        for x, y in tapa_holes:
            add_cylinder(comp, "Poste tornillo tapa", x, y, floor, 7, H - floor - 2)
            cut_round_hole(comp, x, y, 2.2, H + 2)

        # =========================
        # TAPA A LA DERECHA
        # =========================
        offset_x = L + 20
        lid_thick = 3
        lip_h = 2

        add_box(comp, "Tapa superior lisa sin LED", offset_x, 0, 0, L, W, lid_thick)

        # Labio interior tapa
        add_box(comp, "Labio tapa inferior", offset_x + wall + 0.5, wall + 0.5, lid_thick, L - 2*wall - 1, 2, lip_h)
        add_box(comp, "Labio tapa superior", offset_x + wall + 0.5, W - wall - 2.5, lid_thick, L - 2*wall - 1, 2, lip_h)
        add_box(comp, "Labio tapa izquierdo", offset_x + wall + 0.5, wall + 0.5, lid_thick, 2, W - 2*wall - 1, lip_h)
        add_box(comp, "Labio tapa derecho", offset_x + L - wall - 2.5, wall + 0.5, lid_thick, 2, W - 2*wall - 1, lip_h)

        # Agujeros tapa, sin LED
        for x, y in tapa_holes:
            cut_round_hole(comp, offset_x + x, y, 2.2, lid_thick + 2)

        ui.messageBox("Listo: caja 150x80, sin USB, sin LED, solo salida del sensor.")

    except:
        if ui:
            ui.messageBox("Error:\n{}".format(traceback.format_exc()))
