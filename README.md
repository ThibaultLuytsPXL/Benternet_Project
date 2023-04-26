# WarHammer_40K_Imperium_Of_Man_Lore + DiceRoller

In this project i wil be making a service that gives Warhammer 40K Lore using ZMQ to talk between the service and a client using the benternet broker.

## layout

The layout between the client and service is shown here.

![BenternetLayout](BenternetImages/flowchart.png)

The Client sends its message to the broker with the right topic. The broker wil then send the message to the service that listens on said topic. The service wil then respond to that message with a different topic that the client listens to.

## Ladder Structure

This is how the commands between the Client and Service looks with the correct Topics.

![ladder](BenternetImages/ladder.png)

The service responds to the client with a lttle info on what has been asked and (sends you to the website)/ also with links to websites with more information.
The List with all possible info can be found in the list section. 

### Requesting help

The `Lorehammer?>Help` commands shows the possible topic addons for using the service by showing `Lorehammer!>diceroller>(#) or Lore`.

### Using the DiceRoller

Using `Lorehammer?>Dice>#` with # as a number wil return `Lorehammer!>Dice>#` where here # is a number between 0 and the original number.

### Requesting Lore

The command `Lorehammer?>Lore` wil show the three available categories to choose from `Lorehammer!>Loyal, Traitor, Alpha Legion`.

You can then choose which topic you want, for example `Lorehammer?>L>SpaceMarines Legions` with **L** for Loyal, **T** for Traitor and **@** for Alpha Legion *because Alpha Legion*.

## List
- DiceRoller (gives a random number between 0 and the given number)/(also used for *suprise me*)
- Loyal
  - The God Emperor of Mankind
  - Astra Militarum/Imperial Guard
  - Imperial Guard Regiments  
  - Imperial Guard Notable Characters
  - Astartes/SpaceMarines
  - SpaceMarines Legions
  - SpaceMarines Primarchs 
  - SpaceMarines Notable Characters
  - Adeptus mechanicus
  - Imperial Knights
  - Collegia titanica
  - Adeptus custodes
  - Adeptus sororitas
  - The Inquisition
  - Assasins
  - Suprise me
- Traitor
  - Chaos Gods
  - Chaos Demons 
  - Traitor Guard
  - Traitor Guard Regiments 
  - Traitor Guard Notable Characters
  - Chaos SpaceMarine
  - Chaos SpaceMarine Legions 
  - Chaos SpaceMarine Primarchs  
  - Chaos SpaceMarine Notable Characters
  - Dark Mechanicum
  - Suprise me
- Alpha Legion
  - Primarchs
  - Notable Characters
  - Suprise you!
