include <./seal-base.scad>

module start_seal_base() {
    union(){
        difference() {
            seal_base(withTube=true);

            tube_slot();
        }
    }

    module tube_slot() {
        x_translation = seal_base_footprint/2;
        y_translation = seal_base_footprint/2;
        z_translation = tower_slot_depth + actuator_seal_sinking;
        translate([x_translation, y_translation, z_translation]) {       
            cylinder(r=add_two_sided_printer_offset(tube_thickness/2), h=tube_stand_out);
        }
    }
}