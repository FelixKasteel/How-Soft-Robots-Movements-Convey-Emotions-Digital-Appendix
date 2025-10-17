module back_grooves() {
    back_cw_grooves_offset = 2 * groove_spacing / 4;
    back_ccw_grooves_offset = 4 * groove_spacing / 4;
    translate([0, body_depth, body_height])
    rotate([ 180, 0, 0 ]){
        grooves(body_height, back_cw_grooves_offset, back_ccw_grooves_offset);
    }
}