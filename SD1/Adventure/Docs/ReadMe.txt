Saddha Santanaporn
12/7/2017

Known Issues: None

Completed Feature:
- glDrawArrays instead of glBegin
- Map::RenderTiles() optimized
- Two sprites per Tile
- Entity sorted draw order
- Actor-vs-Actor disc corrective
- HeatMap
- Stats
- Combat (fully-featured)
- Items (affect stats) 
- Inventory and Equipment (get, drop, equip)
- Loot (data-driven control)
- MapGenStep_SpawnActor
- MapGenStep_SpawnItem
- MapGenStep_CellularAutomata
- MapGenStep_PerlinNoise
- UI (main menu, inventory menu)
- HUD (health bars, onscreen status info)
- Basic (Dijkstra) pathfinding
- Behavior / Utility system
- Custom features: Run, Consume potion, Select enemy, Transfer an item to enemy

How to Use:
Left joy stick: Move
X: Attack
A: drop item from selected slot/pick up item
Y: select/swap head slot
B: select/swap chest slot
LB: Consume potion
LT: Run
RB: Select enemy
RT: Transfer an item from selected slot to the selected enemy
Back: Stats
Start: Pause/Resume

Deep Learning:
There were several times that I had to go back to the engine code to fix a long lasting bugs that I've somehow never discovered in Incursion.
I also had to go back to and many quality of life improvements for engine code. Making my own engine definitely makes me more deliberate with my code,
since any shortcuts or mistakes I make will eventually hit me in future projects. I remember waiting for an Unity update to fix a crucial bug. 
Now, with my own engine, the responsibility of fixing engine bugs is in my hand; I hope that those bugs will be easy to fix.
