module seal_base(withTube = false){
    base_height = withTube ? seal_base_height : seal_base_height - tube_stand_out;
    difference() {
        seal_base_cube();

        tower_slot_cutout();
        sealing_cutout();
    }

    module seal_base_cube() {
        cube(size=[seal_base_footprint, seal_base_footprint, base_height]);
    }

    module tower_slot_cutout() {
        x_y_translation = seal_base_footprint/2 - seal_tower_footprint/2;
        translate([x_y_translation, x_y_translation, 0]) {   
            cube(size=[add_two_sided_printer_offset(seal_tower_footprint), add_two_sided_printer_offset(seal_tower_footprint), tower_slot_depth]);
        }
    }

    module sealing_cutout() {
        x_translation = seal_base_footprint/2 - actuator_depth/2;
        y_translation = seal_base_footprint/2 - actuator_height/2;
        z_translation = tower_slot_depth;
        translate([x_translation, y_translation, z_translation]) {   
            cube(size=[add_two_sided_printer_offset(actuator_depth), add_two_sided_printer_offset(actuator_height), actuator_seal_sinking]);
        }
    }
}