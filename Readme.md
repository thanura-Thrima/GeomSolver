## Project description:
SELECTING OVERLAPPING GEOMETRIC OBJECTS.
The task is to create a program that selects geometric 2D objects that match a given criteria
from a dataset. The type of objects are Circle, Rectangle and Triangle. The dataset is stored
as comma separated values (csv) files. In the directory SampleData you will find some test
data. One csv-file will contain only one type of objects. The number of geometric objects
could be larger than in the example dataset. You can assume that the whole dataset will fit
into memory.

### Development environment
Visual Studio 2022 community visualc++ std::c++17 

### Running instructions:
geometry.exe [directory] [operation] [input_for_the_operation]
 -directory : Absolute directory
 -operation : OBJECTS_AT_POINT or OVERLAPPING_OBJECTS
 -input_for_the_operation : (x,y) for OBJECTS_AT_POINT

 ### Software Design

Pre-initialized QuadTree data structure for eliminate unnecessary element search.

"Separating Axis Theorem" (SAT) mechanism employed to detect Overlap of convex objects. Circle with convex object overlapping also use a variant of same SAT.
(refence : https://code.tutsplus.com/collision-detection-using-the-separating-axis-theorem--gamedev-169t)

Intersections were determine including error tolerance mechanism and For the simplicity, this is a compile time constant. 
Most of the Key class components and data structures uses Template meta programming technique which can change certain data type in compile time.

Although Software was design in single threaded way, The intention to extend the time consuming algorithms.
 eg: element Overlapping, OBJECTS_AT_POINT and QuadTree implementation can be done in multi-threaded way

### Further work:
Thread pool and multi-threaded capability.
Improvement from simple quadtree structure to complex bounding box structure base Quadtree.

