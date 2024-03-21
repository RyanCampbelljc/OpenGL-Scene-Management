A scene management assignment in OpenGL.
All nodes in the scene have a spherical bounding volume and depending on its world location is placed in a node in a quad tree.
Every frame there is camera frustum culling done against the axis aligned bounding box's in the quad tree, and the nodes in the quadtree sections that passed. This is a much more efficient rendering method than sending every node in the world through the graphics pipeline every frame.

All the code I made is in the src folder.
