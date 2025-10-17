include <./body-mold.scad>
include <./grooves/bottom-grooves.scad>
include <./grooves/back-grooves.scad>
include <./grooves/top-grooves.scad>
include <./grooves/front-grooves.scad>

module grooved_body_mold() {
        body_mold();
        body_mold_grooves();

    module body_mold_grooves() {
        translate([end_cap_overhang + grooves_start_offset, body_mold_depth/2 - body_cavity_depth/2, body_mold_height/2 - body_height/2 ]){
        difference() {
            union()
            {
                bottom_grooves();
                back_grooves();
                front_grooves();
            }
                mold_grooves_cutout();
            }
        }

        module mold_grooves_cutout(){
            translate([-groove_thickness, groove_thickness/2, -groove_thickness/2])
            cube([body_cavity_length, body_cavity_depth - groove_thickness, groove_thickness]);
        }
    }

}