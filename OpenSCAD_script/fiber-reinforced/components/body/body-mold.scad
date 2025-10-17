module body_mold() {
    difference()
    {
        // base cube
        cube([body_mold_length, body_mold_depth, body_mold_height]);

        // body cavity
        translate([0, body_mold_depth / 2 - body_cavity_depth / 2 ])
        cube([body_cavity_length, body_cavity_depth, body_cavity_height]);

        // rod hole
        translate([body_cavity_length + rod_slot_depth/2 - 0.1, body_mold_depth/2, add_printer_offset(air_chamber_height/2) + add_printer_offset(lid_overhang) + (substract_printer_offset(body_height) - add_two_sided_printer_offset(air_chamber_height))/2])
        cube(size = [rod_slot_depth, add_two_sided_printer_offset(air_chamber_depth), add_two_sided_printer_offset(air_chamber_height)], center = true);

        body_base_lid_kerf();
    }

    module body_base_lid_kerf() {
        translate([ 0, (body_mold_rim - lid_overhang), -z_fight_offset ])
        cube(size = [ body_cavity_length, (lid_overhang*2 + body_cavity_depth), lid_overhang + -z_fight_offset]);
    }
}