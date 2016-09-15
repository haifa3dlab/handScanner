use <TimingBelt.scad>
include <params.scad>

//Just create an STL for GT3 Pulley:
// command: openscad -o pulley.stl make_pulley.scad

echo("upper_ring_out_r=", upper_ring_out_r);
drawPulley(2*upper_ring_out_r, false);


