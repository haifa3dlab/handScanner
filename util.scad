epsilon = 0.1;

module ring(in_rad, out_rad, height, fn = 50) {
    difference() {
        cylinder(r = out_rad, h = height, $fn = fn);
        translate([0,0,-epsilon])
           cylinder(r = in_rad, h = height+2*epsilon, $fn = fn);
    }
}

module Y_cylinder(cr, clen) {
    translate([0, clen/2, 0])
      rotate([90, 0, 0])
        cylinder(r = cr, h = clen, $fn = 50);
}

module X_cylinder(cr, clen) {
    translate([-clen/2, 0, 0])
      rotate([0, 90, 0])
        cylinder(r = cr, h = clen, $fn = 50);
}

module belt_slide_stopper(in_r, height, is_below_belt)
{
  if (is_below_belt) {
    cylinder(r1 = in_r + 2*height, r2 = in_r,
           h = height, $fn = 100);
  } else {
    cylinder(r1 = in_r, r2 = in_r + 2*height,
           h = height, $fn = 100);
  }
}

module fasten_plate(x, y, z, hole_r)
{
  // symmetric about y
  translate([-x/2, 0, 0])
    difference() {
      cube([x, y, z]);
      
      // put hole in the middle of each side square:
      translate([y/2, y/2, -epsilon])
        cylinder(r = hole_r, h = z+2*epsilon, $fn = 50);
      
      // put hole in the middle of each side square:
      translate([x - y/2, y/2, -epsilon])
        cylinder(r = hole_r, h = z+2*epsilon, $fn = 50);
    }
}


//tests

//fasten_plate(40, 10, 6, 2.15);