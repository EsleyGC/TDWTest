# AI Programmer Test for The Dark West


## Overview
### Core points:
- Gameplay Ability System integration: All behaviours are implemented as Gameplay Abilities/Effects, allowing for easy tweaking and extension.


- Data Based parameters for all Gameplay Abilities/Effects: All parameters are exposed via Curve Tables, allowing for easy balancing and adjustments without code changes.


- Full State Tree logic for AI behaviours: All AI behaviours are implemented using State Trees, allowing for easy setup and visualization of AI logic.


- Enviroment Query System (EQS) for AI locomotion and target selection: AI uses EQS to determine optimal paths and targets, enhancing their decision-making capabilities.


- Basic Damageable Interface implementation: A simple damageable interface is provided for entities that can take damage, facilitating interaction with the Gameplay Ability System. 
The current implementation is basic and can be extended as needed. In the future, other types of objects and structures can implement this interface to be able to take damage.


- A basic animation system that allow player and AI characters to play animations that try to match the current aim direction. 
A simple montage system with a generic event sender notifies when an animation tries to trigger a specific event (like executing the hitboxes for an attack).

  
- To select the desired State Tree to play (either melee or ranged), a new AI Controller BP is needed. 
This can be changed in the future, in the case that more types of characters with different behaviours are needed. 
In this case, the state tree can select the desired behaviour based on the character type/tag provided.


- The gameplay tags can be used as powerful tools to identify different things in the game, such as character types, abilities, status, current effects, and more.
This allows for a more modular and flexible design, as different systems can interact with each other based on these tags. For now, few things can implement gameplay tags to its full potential, but in the future more systems can be added to take advantage of this feature.


- The UI is extremely basic (made in less than 20 minutes), just to show some info about the player health and the current ability being used. It uses UMG widgets and events fired from the character when the AttributeSet changes the Health value.
We could, if needed, assign a modular system that binds the different attributes to tags and then send events directly from the AttributeSet when any attribute changes. But for now, I believe this is enough for the test.

--------

### Behaviours implemented

I tried to create an AI that tries to avoid being hit, so the EQS system will asks for locations that are far from the player forward direction, but the movement speed is not very high, so the AI will try to strafe and move around the player while trying to keep a certain distance, based on wether it is a melee or ranged character.

The AI will patrol around a narrow area, and when the player is detected, it will try to initiate the combat. We have two sensors working right now: a sight sensor, that will detect the player if it is in front of the AI and not obstructed by any obstacle, and a damage sensor, that will detect the player if it hits the AI with any attack.

Based on weather the AI is seeing the player or not, it will try to either patrol in the Idle state, engage if is seeing the player, or search for the player if the player was recently seen but is not visible anymore.

We have a team system implemented, so AI only attacks the player because it is on a different team. If the AIs are on different teams, they will attack each other as well.

### Setup Instructions

#### How to change values for balancing:
- All Gameplay Abilities and Effects have their parameters exposed via Curve Tables. You can find these tables in the Content Browser under the "Data" folder.
- Most of the damage/status/cooldown/attribute changes can be adjusted by modifying the GameplayEffects that are located in the "Abilities/GameplayEffects" folder.
- AI behaviour parameters can be adjusted in the State Trees located in the "AI/StateTrees" folder. We have an base state tree with Idle/Engage/Search behaviours, and two derived state trees for Melee and Ranged AI that implement the specific attack behaviours.
- EQS queries used by the AI can be found in the "AI/EQS" folder. You can modify the parameters of these queries to change how the AI selects movement locations and targets.