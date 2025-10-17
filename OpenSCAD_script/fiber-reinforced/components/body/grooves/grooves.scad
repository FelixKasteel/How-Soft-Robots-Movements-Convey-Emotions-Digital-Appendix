include <./groove.scad>

module grooves(straight_length, cw_offset, ccw_offset) {
    groove_length = sqrt(pow(straight_length, 2) + pow(groove_spacing/4, 2));
    cw_groove_types = [groove_type_both, groove_type_clockwise];
    show_cw_grooves = is_in_list(groove_type, cw_groove_types);
    ccw_groove_types = [groove_type_both, groove_type_counter_clockwise];
    show_ccw_grooves = is_in_list(groove_type, ccw_groove_types);
    
    straight_groove();

    highest_winding_index = windings - 1;
    for (i=[0:highest_winding_index]) {
        if(show_cw_grooves){
            translate([ i * groove_spacing + cw_offset, 0, 0 ])
            clockwise_groove();
        }

        if(show_ccw_grooves){
            translate([ i * groove_spacing + ccw_offset, 0, 0])
            counter_clockwise_groove();
        }
    }

    translate([ (windings) * groove_spacing, 0, 0 ])
    straight_groove();

    module straight_groove(){
        groove(straight_length);
    }

    
    module clockwise_groove(){
        inclined_groove(groove_length);
        // echo("clockwise_groove:", groove_length);
    }

    module counter_clockwise_groove(){
        color([100/255, 100/255, 0/255]) {
            inclined_groove(groove_length, clockwise_inclination=false);
            // echo("clockwise_groove_counter:", groove_length);
        }
    }

}