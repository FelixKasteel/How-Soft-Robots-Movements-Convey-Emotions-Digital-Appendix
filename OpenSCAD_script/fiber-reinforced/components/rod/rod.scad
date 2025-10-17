module rod(additional_printer_offset=0.0){
    echo(str("Printer Offset = ", $printer_offset));
    cube(size=[air_chamber_length * 1.5, substract_two_sided_printer_offset(air_chamber_depth) - additional_printer_offset, substract_two_sided_printer_offset(air_chamber_height) - additional_printer_offset]);
}