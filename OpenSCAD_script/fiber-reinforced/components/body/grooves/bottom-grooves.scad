module bottom_grooves() {
    bottom_cw_grooves_offset = 1 * groove_spacing / 4;
    bottom_ccw_grooves_offset = 1 * groove_spacing / 4;
    
    translate([0, 0, body_height])
    rotate([ -90, 0, 0 ]) {
        grooves(body_depth, bottom_cw_grooves_offset, bottom_ccw_grooves_offset);
    }
}