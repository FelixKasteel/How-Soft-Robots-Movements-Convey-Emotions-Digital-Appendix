use <fiber-reinforced-actuator-mold.scad>
include <ids.scad>

$fn = 20;

actuatorLength = 80;
actuatorWidth = 30;
actuatorHeight = 20;

actuatorPreview = false;

actuator_size = [actuatorLength, actuatorWidth, actuatorHeight];

mold_parts_to_show = concat([0, seal_tower_id, end_seal_base_id], skin_mold_part_ids);

//mold_parts_to_show hides some parts. Remove the parameter to show all parts. See the code which other parameters are possible
fiber_reinforced_actuator(actuator_size, preview_actuator=actuatorPreview);