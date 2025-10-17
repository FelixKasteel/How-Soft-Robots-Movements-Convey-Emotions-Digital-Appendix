module skin_mold() {
    difference()
    {
        skin_mold_base_cube();

        actuator_cutout();
        skin_mold_rod_hole();
        skin_mold_top_kerf();
    }

    module skin_mold_base_cube(){
        cube([ skinMoldLength, skinMoldDepth, skinMoldHeight ]);
    }

    module actuator_cutout(){
        translate([ 0, skinMoldDepth / 2 - skinCavityDepth / 2 ])
        cube([ skinCavityLength, skinCavityDepth, skinCavityHeight ]);
    }

    module skin_mold_rod_hole(){
        translate([skinCavityLength + rod_slot_depth/2, skinMoldDepth/2, skinMoldHeight/2])
        cube(size = [ rod_slot_depth, air_chamber_depth, air_chamber_height ], center = true);
    }

    module skin_mold_top_kerf(){
        translate([ 0, (skinMoldRim - lid_overhang), 0 ])
        cube(size = [ skinCavityLength, (lid_overhang*2 + actuator_depth), add_printer_offset(lid_overhang) ]);
    }
}