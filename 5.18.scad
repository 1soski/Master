//translate([49,25,8])
union () {
difference () {
cube([33.45,42.85,1.5]);

//Screw Cubes
//top left
translate([3.1,-0.1,-0.1])
cube([4.15,7.5,4.2]);

//bottom left
translate([26.01,-0.1,-0.1])
cube([4.15,7.5,4.2]);

//top right
translate([3.1,35.35,-0.1])
cube([4.15,7.5,4.2]);

//bottom right
translate([26.01,35.35,-0.1])
cube([4.15,7.5,4.2]);

rotate([0,90,0])
translate([-1.5,21.75,2])
cylinder(d=1.5, h=34, $fn=32);

rotate([0,90,0])
translate([-1.5,23.25,2])
cylinder(d=1.5, h=34, $fn=32);

translate([2,21.75,-1.3])
cylinder(d=1.5, h=4.7, $fn=32);

translate([2,23.25,-1.3])
cylinder(d=1.5, h=4.7, $fn=32);
}

translate([0.625,20.375,-3.4])
difference () {
cube([2.5,4,4]);

translate([1.375,1.375,-.1])
cylinder(d=1.5, h=4.7, $fn=32);

translate([1.375,2.875,-.1])
cylinder(d=1.5, h=4.7, $fn=32);


}
}


//Plate for Tubing Pressure
translate([34.725,0, -1])
difference () {
cube([16.725,42.85,0.5]);
    
    translate([9.44,2.5,0])
    cube([4.15,5,4.2]);

    translate([9.44,35.35,0])
    cube([4.15,5,4.2]);    
}






module MiSeqPressurePlate(){
    translate([0,30,0])
union(){
    difference() {
        translate([-50,1.5,2])        //base
            cube([100,38.5,3]);
        //screw openings
        translate([18,5,0])
            cylinder(r=2.5,h=6,$fn=16);
        translate([-18,5,0])
            cylinder(r=2.5,h=6,$fn=16);
        translate([18,22,0])
            cylinder(r=2.5,h=6,$fn=16);
        translate([-18,22,0])
            cylinder(r=2.5,h=6,$fn=16);
        //interface opening
        translate([-12,0,0]) 
            cube([24,29,5]);
    //fitting holes 
        translate([-7,35,0])         
            cylinder(r=3.6,h=6,$fn=6);
        translate([7,35,0])
            cylinder(r=3.6,h=6,$fn=6);
    }
    //gasket holder  
    difference(){
        //base  
        translate([-13.8,1.5,-11])  
            cube([27.6,16,15.5]);
        //gasket slot
        translate([-10.5,6.5,-11])
            cube([21,17.5,1.9]);
        translate([-7,1.5,-11])
            cube([14,17.5,1.9]);
        
        // Cube to widen the rectangular opening from above the chip notch
            translate([-8.25,-1,-4.5]) // Position the cube further up above the chip notch
            cube([16.5, 70, 14.5]); // Adjust the dimensions as needed
        
        // Cube to widen the rectangular opening from above the chip notch
            translate([-15.25,7,-4.5]) // Position the cube further up above the chip notch
            cube([32, 80, 50]); // Adjust the dimensions as needed

        }
        
}}

module MiSeqCradle (){
    color ("purple");
    
    translate([0,30,0])
    union() {
        difference() {
            union(){
                translate([-58.2,0,8])      //base left
                    cube([50,63,3]);
                translate([8.2,0,8])        //base right
                    cube([50,63,3]);
            }
            translate([25,10,7])        //screw openings
                cylinder(r=2.5,h=6,$fn=16);
            translate([-25,10,7])
                cylinder(r=2.5,h=6,$fn=16);
            translate([25,34,7])
                cylinder(r=2.5,h=6,$fn=16);
            translate([-25,34,7])
                cylinder(r=2.5,h=6,$fn=16);
            translate([-6.2,0,6])       //prism opening
                cube([12.4,32,9]);
            translate([-15,12,7])       //heater screw openings
                cylinder(r=2.2,h=6,$fn=16);
            translate([15,12,7])       
                cylinder(r=2.2,h=6,$fn=16);   
        }
        difference() {
            translate([-12.2,0,-3])
                cube([24.4,63,12]);
            translate([-6,0,-4])        //scope opening
                cube([12,25,12]);
            translate([0,26,-7])
                cylinder(r=6.1,h=6.2,$fn=16);
            translate([-7.5,0,-2]) 
                cube([15,28,1.5]);      //chip notch
            translate([-6.2,0,-0.7]) 
                cube([12.4,28,9.7]);    //prism opening
            translate([-4,28,-0.9])     //top laser bevel
                rotate([30,0,0]){
                    cube([8,35,7.7]);
            }  
            // Cube to widen the rectangular opening from above the chip notch
            translate([-8.25,-1,-0.5]) // Position the cube further up above the chip notch
                cube([16.5, 70, 10]); // Adjust the dimensions as needed
        }  
        
    }
}


translate([23.5,10.5,9])
    rotate([0,0,90]){
    MiSeqPressurePlate();}
translate([21,3.5,0.5])

translate([24,10.5,2.5])
    rotate([0,0,-90]){
        MiSeqCradle();}
