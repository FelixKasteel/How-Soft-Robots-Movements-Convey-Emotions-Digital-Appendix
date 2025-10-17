module mug() {
    translate([0, 100, 0])
    scale([.3, .3, .3])
    rotate([90, 0, 120])
    color([1,1,1]) 
    {
        import("mug.stl", convexity=3, center=false);
    }
}