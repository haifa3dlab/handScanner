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
  ring_h = ring_base_w;
  ring_elev = lower_ring_h - 2*glassBall_r;
  union() {
    translate([0, 0, ring_elev + ring_h - epsilon])
      difference() {
        union() {
          // belt slide stopper from above:
          belt_slide_stopper(upper_ring_out_r, wall_w, true);
  
          translate([0, 0, wall_w - epsilon])
            scale([1, 1, 1.2])
              import("pulley.stl", convexity=4);
              // create using make_pulley.scad
              // drawPulley(2*upper_ring_out_r, false);
              // or: cylinder(r=upper_ring_out_r, h=7.8);
  
          // belt slide stopper from above:
          translate([0, 0, wall_w + pulley_real_h])
            belt_slide_stopper(upper_ring_out_r, wall_w, false);        
        }
        pulley_part_h = pulley_real_h + 2*wall_w;
    
        // central hole:
        translate([0, 0, pulley_part_h/2 - epsilon]) 
          union() {
            cube([profile2020_w, profile2020_w,
                  pulley_part_h + 1], center = true);
    
            rotate([0, 0, 45])
            translate([(mainHole_r + profile2020_w)/2, 0, 0]) // middle of wall
              cube([usb_hole_w, usb_hole_l,
                    pulley_part_h + 1], center = true);
          }
        }
    intersection() {
      difference() {
        translate([0, 0, ring_elev])
          ring(mainHole_r, 
               upper_ring_out_r,
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
