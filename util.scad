epsilon = 0.1;

module ring(in_rad, out_rad, height, fn = 50) {
    difference() {
        cylinder(r = out_rad, h = height, $fn = fn);
        translate([0,0,-epsilon])
           cylinder(r = in_rad, h = height+2*epsilon, $fn = fn);
    }
    
}

