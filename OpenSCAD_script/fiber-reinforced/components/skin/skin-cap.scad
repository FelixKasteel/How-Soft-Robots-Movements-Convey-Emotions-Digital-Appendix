module skin_cap() {
    capThickness = 2 * end_cap_overhang;
    difference() {
        skin_cap_base();
        skin_cap_rod_hole();
    }

    module skin_cap_base() {
        union() {
            cube(size = [ capThickness, skinMoldDepth, skinMoldHeight + lid_thickness - lid_overhang]);

            translate([capThickness, skinMoldDepth/2 - substract_two_sided_printer_offset(actuator_depth)/2, lid_overhang])
            cube(size = [end_cap_overhang, substract_two_sided_printer_offset(actuator_depth), substract_two_sided_printer_offset(actuator_height)]);
        }
    }

    module skin_cap_rod_hole() {
        translate([capThickness/2 + end_cap_overhang/2, skinMoldDepth/2, skinMoldHeight/2])
        cube(size = [ capThickness + end_cap_overhang, add_two_sided_printer_offset(air_chamber_depth), add_two_sided_printer_offset(air_chamber_height) ], center = true);
    }
}