/*
 * PROJ1-1: YOUR TASK B CODE HERE
 *
 * Feel free to define additional helper functions.
 */

#include <stdlib.h>
#include <stdio.h>
#include "quadtree.h"
#include "make_qtree.h"
#include "utils.h"

#define ABS(x) (((x) < 0) ? (-(x)) : (x))
/*

map_width is the


*/
int homogenous(unsigned char *depth_map, int map_width, int x, int y, int section_width) {

// now get to correct xposA NOte that x positions are counted horizontally
// and y positions are vertically
// so correct index to start at is image_width * (y)+ x where x and y
int startpos= map_width * y +x;
int endpos= startpos+  map_width*(section_width -1) +( section_width-1); // section width must be >=1 lower right corner
unsigned char pixel_val= *(depth_map+ startpos);
int xcounter=0; // loop var
unsigned int i;
for ( i=(startpos); i<= endpos; i++)
{
if(xcounter==section_width)  // go to next line
{
xcounter=0;
i--;  // to get to end of row of feature ( not row of image)
i+=map_width ;// go to next line;
i-= ( section_width-1);// go to beginning of section ( on next line)
i--; /// since i will iterate one at end of loop
    continue;
}
if ( pixel_val!= *(depth_map+i))
        return 256;  // do not maccht
xcounter++;

}
return (int) pixel_val;
// we need to go to startpos + sectio_ n width and startpos + section_width
// in both x and y directions  This is equivalent toa map width*ssection_width
// + section _widthe

}
qNode * makeChildNode( unsigned char * depth_map, int map_width,
        int x, int y, int section_width )
{
  qNode * quad_tree=(qNode *)  malloc ( sizeof(qNode ) );
if (quad_tree== NULL)
{
allocation_failed();
return NULL;
}

int pixelVal=homogenous(depth_map, map_width, x,y,section_width);
quad_tree->size= section_width;
quad_tree->x = x;
quad_tree->y = y;
quad_tree->gray_value=pixelVal;
if(pixelVal!=256)
{
quad_tree->leaf=1;
quad_tree->child_NW=NULL;
quad_tree->child_NE=NULL;
quad_tree->child_SE=NULL;
quad_tree->child_SW=NULL;
return quad_tree;

}
else
{
  quad_tree->leaf=0;

  quad_tree->child_NW=makeChildNode(depth_map,map_width,x,y, section_width/2);
  quad_tree->child_NE=makeChildNode(depth_map, map_width, x+section_width/2, y , section_width/2);
quad_tree->child_SW= makeChildNode(depth_map,  map_width, x, y + (section_width/2), section_width/2);
quad_tree->child_SE=makeChildNode(depth_map, map_width, x +section_width/2, y+ ( section_width/ 2) , section_width/2);


}
return quad_tree;
}

qNode *depth_to_quad(unsigned char *depth_map, int map_width) {

qNode * quad_tree= (qNode*)  malloc ( sizeof(qNode) );
if (quad_tree==NULL)
{
   allocation_failed();
   return  NULL;

}
int pixelVal= homogenous(depth_map, map_width,0,0,map_width);
quad_tree->size=map_width;
quad_tree->x=0;
quad_tree->y=0;
quad_tree->gray_value=pixelVal;


if( pixelVal!=256)
{
quad_tree->leaf=1;
quad_tree-> child_NW= NULL;
quad_tree->child_NE= NULL;
quad_tree->child_SE=NULL;
quad_tree->child_SW=NULL;
return quad_tree;

}

else
{
quad_tree->leaf=0;
quad_tree->child_NW= makeChildNode( depth_map, map_width, 0,0, map_width/2);
quad_tree->child_NE= makeChildNode( depth_map, map_width,map_width/2 ,0, map_width/2);
quad_tree->child_SW= makeChildNode( depth_map, map_width,  0,map_width/2, map_width/2);
quad_tree->child_SE= makeChildNode( depth_map, map_width, map_width/2,map_width/2,  map_width/2);



}
return quad_tree;
}
void free_qtree(qNode *qtree_node) {
    if(qtree_node) {
        if(!qtree_node->leaf){
            free_qtree(qtree_node->child_NW);
            free_qtree(qtree_node->child_NE);
            free_qtree(qtree_node->child_SE);
            free_qtree(qtree_node->child_SW);
        }
        free(qtree_node);
    }
}



