module front_grooves(){
    font_cw_grooves_offset = 0 * groove_spacing/4;
    font_ccw_grooves_offset = 2 * groove_spacing/4;
        rotate([0, 0, 0]){
        grooves(body_height, font_cw_grooves_offset, font_ccw_grooves_offset);
            echo(str("Body length: ", body_height));
    }
}