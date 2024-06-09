// Parameters
plate_length = 47.5; // in mm (4.4 cm)
plate_width = 47.5;  // in mm (4.4 cm)
plate_thickness = 4; // in mm
line_length = 47.5; // in mm, equal to the plate width
line_width = 1.5; // in mm
line_height = 5.5; // in mm
hole_diameter = 23; // Diameter of the hole
hole_diameter2 = 26.5; // Diameter of the hole

// Main plate with lines and hole on the opposite side
difference() {
    // Main plate
    cube([plate_length, plate_width, plate_thickness]);

    // Circular opening through the plate, centered along the width but on the opposite face
    translate([plate_length / 2, plate_width / 2, plate_thickness])
        cylinder(h=plate_thickness + 2, d=hole_diameter2, center=true);
    
    translate([plate_length / 2, plate_width / 2, plate_thickness/2])
        cylinder(h=plate_thickness, d=hole_diameter, center=true);
}



// Add two lines at the bottom of the structure, at the ends
translate([0, 0, -2])
    cube([line_length, line_width, line_height]);

translate([0, plate_width - line_width, -2])
    cube([line_length, line_width, line_height]);
