module groove(length){
    // cylinder(h = groove_length, d = groove_thickness);
    if (length == undef){
        echo("undef length groove");
    }
    translate([0, -groove_thickness/2, 0])
    cube(size=[groove_thickness, groove_thickness, length], center=false);
//    echo(length);
}

module inclined_groove(length, clockwise_inclination=true){
    if (length == undef){
        // echo("undef length inclined groove");
    }
        counter_clockwise_factor = clockwise_inclination ? 1 : -1;
    rotate([ 0, groove_inclination * counter_clockwise_factor, 0 ])
    groove(length);
}

