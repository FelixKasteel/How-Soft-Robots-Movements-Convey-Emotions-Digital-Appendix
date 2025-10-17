module seal_tower() {
    difference() {
        seal_tower_cube();
        seal_tower_cutout();
    }
    
    module seal_tower_cube() {
        cube(size=[substract_two_sided_printer_offset(seal_tower_footprint)-0.05, substract_two_sided_printer_offset(seal_tower_footprint)-0.05, seal_tower_height], center=true);
    }

    module seal_tower_cutout() {
        cube(size=[add_two_sided_printer_offset(actuator_depth), add_two_sided_printer_offset(actuator_height), actuator_length], center=true);
    }
}