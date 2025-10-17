module body_lid() {
    lid_base();
    lid_grooves();

    module lid_base() {
        difference()
            {
                // base cube
                cube(size = [ body_mold_length + lid_grab_overhang, body_mold_depth, lid_thickness ]);

                // right kerf
                cube(size = [ body_mold_length, add_printer_offset(body_mold_rim - lid_overhang), lid_overhang ]);

                // left kerf
                translate([ 0, substract_printer_offset(body_mold_depth - body_mold_rim + lid_overhang), 0 ])
                cube(size = [ body_mold_length, add_printer_offset(body_mold_rim - lid_overhang), lid_overhang ]);

                // back kerf
                translate([ substract_printer_offset(body_cavity_length), 0, 0 ])
                cube(size = [ add_printer_offset(body_mold_rim + lid_grab_overhang), body_mold_depth, lid_overhang ]);
            }
    }

    module lid_grooves() {
        translate([end_cap_overhang + grooves_start_offset, body_mold_rim, 0 ])
        {
            difference(){
                flipped_top_grooves();
                top_grooves_cutouts();
            }
        }

        module flipped_top_grooves(){
            translate([0,body_depth,0])
            rotate([180, 0, 0]){
                top_grooves(); 
            }
        }

        module top_grooves_cutouts(){
            translate([-groove_thickness, 0, -groove_thickness]) {
                difference() {
                    translate([0, -$printer_offset/2, 0]) 
                    cube([body_cavity_length, body_cavity_depth + $printer_offset, groove_thickness*2]);

                    translate([0, groove_thickness/2, groove_thickness/2])
                    cube([body_cavity_length, body_cavity_depth - groove_thickness, groove_thickness]);
                }   
            }
        }
    }
}