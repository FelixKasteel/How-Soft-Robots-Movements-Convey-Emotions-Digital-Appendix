//FRA defined by body dimensions and air chamber dimensions
use <arrange/arrange.scad>
use <mug/mug.scad>
use <rounded_cube/roundedcube_simple.scad>

include <ids.scad>

$printer_offset = .15;
z_fight_offset = .001;

module fiber_reinforced_actuator(
    actuator_size=[85.4, 11, 11],
    wall_thickness=2,
    seal_thickness = 5,
    twist=false,
    clockwise_twist=true,
    groove_spacing = 3,
    mold_parts_to_show=all_mold_part_ids,
    preview_actuator=true,
    visualise_dimension=true,
    silent=false){
    end_cap_overhang = 2;
    lid_overhang = 2;
    rod_slot_depth = 3;
    lid_grab_overhang = 3;
    default_rim = 7.5;

    actuator_length = actuator_size[0];
    actuator_depth = actuator_size[1];
    actuator_height = actuator_size[2];

    body_wall_thickness = wall_thickness/2;
    skin_thickness = wall_thickness/2;

    air_chamber_length = actuator_length - 2 * seal_thickness;
    air_chamber_depth = actuator_depth - 2 * wall_thickness;
    air_chamber_height = actuator_height - 2 * wall_thickness;
    air_chamber_size = [air_chamber_length, air_chamber_depth, air_chamber_height];

    needle_cap_height = 18.5;
    needle_cap_diameter = 7.6;
    needle_foot_height = 4;
    needle_foot_diameter = 4;

    seal_tower_thickness = 5;
    seal_base_rim = default_rim + seal_tower_thickness * 2;
    seal_base_footprint = (actuator_depth + actuator_height) / 2 + seal_base_rim * 2;
    //defines how deep the actuator sinks into the sealing (e.g. sela thickness is 8 mm a sinking factor of .5 leds to an actuator where the sealing is half added and half filled the air channel by 4 mm each)
    seal_adding_factor = .5;
    seal_base_sinking_factor = .5;
    additive_seal_height = seal_thickness * seal_adding_factor;
    air_channel_seal_height = seal_thickness - additive_seal_height;
    displacement_factor = .5;

    body_wrapping_expansion = 1; // necessary to prevent squeezed actuator in skinning step
    body_length = actuator_length - seal_thickness*2 - body_wrapping_expansion;
    body_depth = actuator_depth - 2 * skin_thickness;
    body_height = actuator_height - 2 * skin_thickness;

    body_cavity_length = body_length + end_cap_overhang;
    body_cavity_depth = body_depth;
    body_cavity_height = body_height + lid_overhang;

    floor_thickness = 2;
    body_mold_rim = default_rim;
    body_mold_length = body_cavity_length + body_mold_rim;
    body_mold_depth = body_cavity_depth + 2 * body_mold_rim;
    body_mold_height = body_cavity_height + floor_thickness;

    lid_thickness = 3 * lid_overhang;

    groove_thickness = 0.4;
    body_circumference = 2 * body_height + 2 * body_depth;
    groove_inclination = atan(groove_spacing / body_circumference);
    groove_length = sqrt(pow(body_depth, 2) + pow(groove_spacing/4, 2));
//    groove_length_height = sqrt(pow(body_height, 2) + pow(groove_spacing/4, 2));
    grooves_start_offset = 1;
    grooves_end_offset = 1;
    windings_length = body_length - grooves_start_offset - grooves_end_offset;
    windings_divisional_rest = (windings_length - groove_thickness)%groove_spacing;
    windings_add_up = windings_divisional_rest == 0;
    if (!windings_add_up) {
        echo(str("Your body is either ", windings_divisional_rest, "mm too long or ", groove_spacing - windings_divisional_rest, "mm too short."));
    }
    windings = floor((windings_length - groove_thickness)/groove_spacing);

    groove_type_both = 0;
    groove_type_clockwise = 1;
    groove_type_counter_clockwise = 2;

    groove_type = !twist ? groove_type_both : clockwise_twist ? groove_type_clockwise : groove_type_counter_clockwise;

    include <./components/body/grooved-body-mold.scad>
    include <./components/body/body-lid.scad>
    include <./components/body/body-cap.scad>

    skinCavityLength = actuator_length + end_cap_overhang - seal_thickness*2;
    skinCavityDepth = actuator_depth;
    skinCavityHeight = actuator_height + lid_overhang;

    skinMoldRim = default_rim;
    skinMoldBottomTickness = floor_thickness;
    skinMoldLength = skinCavityLength + skinMoldRim;
    skinMoldDepth = skinCavityDepth + 2 * skinMoldRim;
    skinMoldHeight = skinCavityHeight + skinMoldBottomTickness;

    tube_thickness = 3.2;
    tube_stand_out = 30;
    tower_slot_depth = 5;
    actuator_seal_sinking = seal_thickness;
    //educated guess how much silicone needs to be displaced inside the sealing base to let it rise inside the air channel the desired amout
    seal_base_height = 
        floor_thickness + 
        tube_stand_out + 
        actuator_seal_sinking + 
        tower_slot_depth;
    seal_tower_height = actuator_length - actuator_seal_sinking * 2 - actuator_length * 0.3;
    seal_tower_footprint = seal_tower_thickness * 2 + (actuator_depth + actuator_height)/2;

    include <./components/actuator/actuator-preview.scad>

    include <./components/skin/skin-mold.scad>
    include <./components/skin/skin-lid.scad>
    include <./components/skin/skin-cap.scad>
    include <./components/rod/rod.scad>
    include <./components/rod/rod.scad>

    include <./components/sealing/seal-tower.scad>
    include <./components/sealing/start-seal-base.scad>
    include <./components/sealing/end-seal-base.scad>

    if(preview_actuator){
        actuator_preview();
        translate([0, actuator_depth + 10, 0]) {
            cutted_actuator_preview();
        }
        if (visualise_dimension) {
            mug();
        }
    }
    else {
        rotate([180, 0, 0]) {
            actuator(actuator_size, air_chamber_size, mold_parts_to_show, skin_thickness);
        }
    }

    if(!silent) {
        print_actuator_dimensions(actuator_size, air_chamber_size, wall_thickness, seal_thickness, body_wall_thickness, skin_thickness, body_length);
    }

    module actuator(actuator_size, air_chamber_size, mold_parts_to_show, skin_thickness) {
        arrange(spacing=56, n=1){
            if(is_in_list(body_mold_id, mold_parts_to_show)) {
                grooved_body_mold();
            }
            if(is_in_list(body_lid_id, mold_parts_to_show)) {
                body_lid();
            }
            if(is_in_list(body_cap_id, mold_parts_to_show)) {
                body_cap();
            }
            if(is_in_list(skin_mold_id, mold_parts_to_show)) {
                skin_mold();
            }
            if(is_in_list(skin_lid_id, mold_parts_to_show)) {
                skin_lid();
            }
            if(is_in_list(skin_cap_id, mold_parts_to_show)) {
                skin_cap();
            }
            if(is_in_list(rod_id, mold_parts_to_show)) {
                rod();
            }
            if(is_in_list(seal_tower_id, mold_parts_to_show)){
                seal_tower();
            }
            if(is_in_list(start_seal_base_id, mold_parts_to_show)){
                start_seal_base();
            }
            if(is_in_list(end_seal_base_id, mold_parts_to_show)){
                end_seal_base();
            }
            if(is_in_list(additonal_seal_tower_id, mold_parts_to_show)){
                seal_tower();
            }
            if(is_in_list(thiner_rod_id, mold_parts_to_show)) {
                rod(additional_printer_offset=0.05);
            }
            if(is_in_list(thicker_rod_id, mold_parts_to_show)) {
                rod(additional_printer_offset=0.1);
            }
        }
    }
}

module print_actuator_dimensions(actuator_size, air_chamber_size, wall_thickness, seal_thickness, body_wall_thickness, skin_thickness, body_length){
    echo(str("Actuator Dimensions (X x Y x Z): ", actuator_size[0], " x ", actuator_size[1]," x ", actuator_size[2]));
    echo(str("Air Chamber Dimensions (X x Y x Z): ", air_chamber_size[0], " x ", air_chamber_size[1], " x ", air_chamber_size[2]));
    echo(str("Body length: ", body_length));
//    echo(str("Body length: ", body_depth));
//    echo(str("Body length: ", body_height));
    echo(str("Total Wall Thickness (Top/Bottom, Front/Back): ", wall_thickness, ", ", wall_thickness));
    echo(str("Body Wall Thickness (Top/Bottom, Front/Back): ", body_wall_thickness, ", ", body_wall_thickness));
    echo(str("Skin Thickness (Top/Bottom, Front/Back): ", skin_thickness, ", ", skin_thickness));
    echo(str("Seal Thickness (Start, End): ", seal_thickness, ", ", seal_thickness));
    echo(str("Printer offset: ", $printer_offset));
    echo(str("All measurements in milimeters"));
}

function is_in_list(element, list) = len(search(element, list)) > 0;

function add_printer_offset(length) = length + $printer_offset;
function add_two_sided_printer_offset(length) = add_printer_offset(add_printer_offset(length));
function substract_printer_offset(length) = length - $printer_offset;
function substract_two_sided_printer_offset(length) = substract_printer_offset(substract_printer_offset(length));