include <./grooves.scad>

module top_grooves() {
    top_cw_grooves_offset = 3 * groove_spacing / 4;
    top_ccw_grooves_offset = 3 * groove_spacing / 4;
    translate([0, body_depth, 0])
    rotate([ 90, 0, 0 ]){
        grooves(body_depth, top_cw_grooves_offset, top_ccw_grooves_offset);
    }
}