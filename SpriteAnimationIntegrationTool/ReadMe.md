# Sprite Animation Integration Tool
Unreal Engine Plugin for integrating sprite animations into the project



## Setup
1. Put in the Plugins folder:
2. Build and can use right away!
3. Find in Window->SpriteAnimationIntegrationTool

## Actor Setup
1. On the Sprite to animate, give it a component tag
   1a. In the Sprite Animation Component, you need to type in the same name as the tag you gave in the sprite tag box
2. Select the amount of directions in the animation
3. You will need to update the state manually
   3a. SpriteAnimation->SetState("State")
   3b. All "State" if they have a space " " should be used with an underscore "_"
4. This works with the direction arrow, so make sure to update that for the directions the character is facing



