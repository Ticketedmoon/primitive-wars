A system perhaps for collision would usually mark up colliding entities, and then separate systems would handle those collisions.  

For instance: 
- A collision system may mark an entity as having collided with another entity.  
- A second system would look for all entities marked as having health with collisions and test to see if the collider is damaging.
- A third system might look for players with collisions and test to see if the collider is a monster, etc.  

Alternatively, you might want to store collisions as entities themselves, so that each time you detect a collision you  
create an entity with a collision component that references the two colliding entities. You can then handle these  
entities like any other in collision-type-specific systems again.  

Regarding your "type components", you probably want to be using your existing components almost as though they were  
types; so you don't have a monster "type", but you might have a Health component, and you should act as though that were  
a sort of "type". So when you're looking for collisions, you're not looking for "monsters" hitting "bullets", but "  
things that have health" and "things that do damage when they hit things".