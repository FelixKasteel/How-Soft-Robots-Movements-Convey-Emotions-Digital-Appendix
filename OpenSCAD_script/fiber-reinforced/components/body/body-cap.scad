module body_cap() {
    capThickness = 2 * end_cap_overhang;
    difference() {
        cap_base();
        rod_hole();
    }

    module cap_base() {
        union() {
            cube(size = [ capThickness, body_mold_depth, body_mold_height + lid_thickness - lid_overhang]);

            translate([capThickness, add_printer_offset(body_mold_depth/2 - body_cavity_depth/2), add_printer_offset(lid_overhang)]) {
            cube(size = [ end_cap_overhang, substract_two_sided_printer_offset(body_cavity_depth), substract_two_sided_printer_offset(body_cavity_height - lid_overhang)]);

            }
        }
    }

    module rod_hole(){
        translate([capThickness/2 + end_cap_overhang/2, body_mold_depth / 2, body_mold_height / 2])
        cube(size = [capThickness + end_cap_overhang, add_printer_offset(add_two_sided_printer_offset(air_chamber_depth)), add_printer_offset(add_two_sided_printer_offset(air_chamber_height)) ], center = true);
    }
}