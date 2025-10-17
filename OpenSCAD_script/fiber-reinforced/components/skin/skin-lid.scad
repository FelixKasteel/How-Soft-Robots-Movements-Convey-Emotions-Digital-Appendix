module skin_lid() {
    difference()
    {
        skin_lid_base();

        skin_lid_right_kerf();
        skin_lid_left_kerf();
        skin_lid_back_kerf();
        }
    module skin_lid_base() {
        cube(size = [ skinMoldLength + lid_grab_overhang, skinMoldDepth, lid_thickness ]);
    }

    module skin_lid_right_kerf(){
        cube(size = [ skinMoldLength, add_printer_offset(skinMoldRim - lid_overhang), lid_overhang ]);
    }

    module skin_lid_left_kerf(){
        translate([ 0, substract_printer_offset(skinMoldDepth - skinMoldRim + lid_overhang), 0 ])
        cube(size = [ skinMoldLength, add_printer_offset(skinMoldRim - lid_overhang), lid_overhang ]);

    }

    module skin_lid_back_kerf() {
        translate([ substract_printer_offset(skinCavityLength), 0, 0 ])
        cube(size = [ add_printer_offset(skinMoldRim + lid_grab_overhang), skinMoldDepth, lid_overhang ]);
    }
}