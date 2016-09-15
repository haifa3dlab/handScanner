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

module lower_ring_fasten_plate(l, total_w, h)
{
  w = big_washer_d + 2;

  //plates with M4 holes to fasten to prof2020
  translate([0, -total_w/2, 0])
    fasten_plate(l, w, h, M4hole_d/2); 
  
  translate([0, total_w/2 - w, 0])
    fasten_plate(l, w, h, M4hole_d/2); 

  ring(total_w/2 - 3*wall_w, total_w/2, h);
}


module lower_ring() {
  difference() {
    union() {     
      ring(bigger_hole_r, 
           lower_ring_max_r,
           lower_ring_h, fn = 100);

      translate([0, 0, -fasten_plate_h])
        lower_ring_fasten_plate(
          2*lower_ring_max_r + big_washer_d, 
          2*lower_ring_max_r,
          fasten_plate_h + epsilon);
    }

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


color("green")
  lower_ring_fasten_plate(
            2*lower_ring_max_r + big_washer_d, 
            2*lower_ring_max_r,
            fasten_plate_h + epsilon);

//color("green") lower_ring();

//color("red") upper_ring();

//color("black") torusForBalls();

