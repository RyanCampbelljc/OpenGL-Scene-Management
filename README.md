# Overview
This project implements a scene management system in OpenGL using a quad tree structure and frustum culling for optimized rendering. Each object in the scene is represented by a node with a spherical bounding volume. Each top level node in a heiarchy is placed in the quadtree.

# Key Features
* **Node Management:** Renderable objects(nodes) in the scene are created in heiarchies. This makes it easy to move and scale related nodes. Bounding volumes that cover an entire heiarchy can then be checked if they are in view, rather than checking each individual node in the world. 
* **Quad Tree Scene Management:** Each top level node is inserted in the quadree. The axis aligned bounding volume of the quadtree can be checked against the cameras view frustum in order to see if any of that quadtrees nodes are possibly in camera view.
* **Spherical Bounding Volume Culling:** Each node and heiarchy of nodes have their own spherical bounding volume. The nodes that are contained in the quadtree sections that passed the prior test are then checked against the cameras view frustum for further culling.
