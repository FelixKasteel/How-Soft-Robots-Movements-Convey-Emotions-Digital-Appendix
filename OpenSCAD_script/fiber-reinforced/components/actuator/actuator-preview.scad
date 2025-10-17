module actuator_preview() {
    difference() {
        union(){
            color([49/255, 156/255, 109/255]) {
                cube(size=[actuator_length, actuator_depth, actuator_height], center=true);
            }
        }
        cube(size=[air_chamber_length, air_chamber_depth, air_chamber_depth], center=true);
    }
}

module cutted_actuator_preview() {
    difference() {
        actuator_preview();
        translate([actuator_length/3, 0, 0]) {
            cube(size=[actuator_length/2 + 1, actuator_depth + 1, actuator_height + 1], center=true);
        }
    }
    
    difference() { 
        union(){
            difference() {
                color([255/255, 255/255, 255/255]) {
                    cube(size=[body_length, body_depth, body_height], center=true);
                }

                cube(size=[air_chamber_length, air_chamber_depth, air_chamber_depth], center=true);
            }
            translate([0, -body_depth/2, -body_height/2]) {   
                top_grooves();
                bottom_grooves();
                front_grooves();
                back_grooves();
            }
        }

        translate([actuator_length/2 + actuator_length/4, 0, 0]) {
            cube(size=[actuator_length, actuator_depth + 1, actuator_height + 1], center=true);
        }
    }
}