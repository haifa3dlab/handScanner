epsilon = 0.1;

module ring(in_rad, out_rad, height, fn = 50) {
    difference() {
        cylinder(r = out_rad, h = height, $fn = fn);
        translate([0,0,-epsilon])
           cylinder(r = in_rad, h = height+2*epsilon, $fn = fn);
    }
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
