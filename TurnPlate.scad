include <params.scad>
use <TimingBelt.scad>
include <util.scad>
use <torusPart.scad>


module torusForBalls() {
  // bigR is in the middle of the lower_ring:
  bigR = mainHole_r + glassBall_r + wall_w;
  cutR = glassBall_r;
  translate([0, 0, cutR + lower_ring_h - 2*glassBall_r])
    torus(bigR, cutR);
}

module ring_cut_conus() {
	conus_h = max_r * tan(ring_contact_angle);
	cylinder(h = conus_h, r1 = 0, r2 = max_r);
}

module lower_ring() {
  difference() {
    ring(bigger_ring_r, 
         bigger_ring_r + ring_base_w,
         lower_ring_h, fn = 100);

    torusForBalls();

    translate([0, 0, -wall_w])
		ring_cut_conus();
  }
}

module upper_ring() {
  ring_out_r = mainHole_r + ring_base_w;
  ring_h = ring_base_w;
  ring_elev = lower_ring_h - 2*glassBall_r;
  union() {
    translate([0, 0, ring_elev + ring_h - epsilon])
      difference() {
        scale([1, 1, 1.2])
          drawPulley(2*ring_out_r, false);
        
        translate([0, 0, +9])
          cylinder(r = mainHole_r, h = 20, center = true, $fn = 50);
      }
    
    intersection() {
      difference() {
        translate([0, 0, ring_elev])
          ring(mainHole_r, 
               ring_out_r,
               ring_h,
               fn = 100);
  
        torusForBalls();
      }
      translate([0, 0, -wall_w + ring_diff_h])
        ring_cut_conus();
    }
  }
}


//color("green") lower_ring();

color("red") upper_ring();

//color("black") torusForBalls();
