![Windows](https://img.shields.io/badge/Windows-0078D6?style=for-the-badge&logo=windows&logoColor=white) ![C++](https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white)  ![Unreal Engine](https://img.shields.io/badge/unrealengine-%23313131.svg?style=for-the-badge&logo=unrealengine&logoColor=white)
# Project Monster Hunter World
A 3D multi-player action game developed with Unreal Engine 5 (C++ and blueprints).  
|   |   |
|---|---|
|__Development period__| April 2023 - May 2023 |
|__Team information__ | 1 server programmer + 4 client programmers (UI/Character/Monster/Item+VFX) |    

_Original repository: [AZ_MHW](https://github.com/AZ-KGCA/AZ_MHW/tree/develop_temp)_

<br />

### Features
* Class generation and data retrieval from CSV
  * [Table Manager](Source/AZ_MHW/Manager/AZTableMgr.h), [Monster Manager](Source/AZ_MHW/Manager/AZMonsterMgr.h)
  * [Monster Data](RawContent/Csv/MonsterData.csv), [Non-combat Data](RawContent/Csv/MonsterNonCombatActionData.csv), [Combat Data](RawContent/Csv/MonsterCombatActionData.csv)
* Multi-player with synchronisation (character, monster, item)
  * Monster classes: [Monster.h](Source/AZ_MHW/Character/Monster/AZMonster.h), [Monster_Client.h](Source/AZ_MHW/Character/Monster/AZMonster_Client.h)
  * Monster packets: [PacketFunction.h](Source/AZ_MHW/SocketHolder/PacketFunction.h), [MonsterPacket.h](Source/AZ_MHW/SocketHolder/Monster/MonsterPacket.h)
* Behaviour tree and custom tasks
  * 3 non-combat actions, 11 combat actions 
  * [Behaviour tree decorators, services, and tasks](Source/AZ_MHW/BehaviorTree)
* Damage exchange between monster and players (damage differs by the damaged body part and the attack type)
  * [MonsterHealthComponent.h](Source/AZ_MHW/CharacterComponent/AZMonsterHealthComponent.h) 
* Dynamic mesh alteration (can sever and/or damage body parts by damaging specific body parts)
  * [MonsterMeshComponent.h](Source/AZ_MHW/CharacterComponent/AZMonsterMeshComponent.h), [MonsterMeshComponent_Client.h](Source/AZ_MHW/CharacterComponent/AZMonsterMeshComponent_Client.h) 
* Aggro system (consideration of damage dealt, distance, body part sever, random)
  * [MonsterAggroComponent.h](Source/AZ_MHW/CharacterComponent/AZMonsterAggroComponent.h)
* Dynamic load of a streaming map
* Animation notifies and custom handlers
* Component pattern (health component, aggro component, etc)
* Singleton pattern (data manager, monster manager, etc)
* Event systems (On body part change (monster), on player enter, etc) 
* Special effects (particle effects for monster skills, player attacks, player footsteps + camera shake and screen distortion for Roar skill)

<br />

### Gameplay Video
[![InGame Video](https://img.youtube.com/vi/b8ydRoG0W_8/0.jpg)](https://www.youtube.com/watch?v=b8ydRoG0W_8)

### Preview
|  |   |
|---|---|
|Main Screen|Generate Player|
|<img width="450" alt="image" src="https://github.com/xpsa0421/Project-Monster-Hunter-World/assets/71711432/ca182b56-f7c0-48b6-a2e1-440c586bfe9d">|<img width="450" alt="image" src="https://github.com/xpsa0421/Project-Monster-Hunter-World/assets/71711432/80331ed8-da33-4d7f-b581-c437e91ecb20">|
|Lobby Map|Combat Map|
|<img width="450" alt="image" src="https://github.com/xpsa0421/Project-Monster-Hunter-World/assets/71711432/6279d1aa-1053-4fdf-b531-440944058f82">|<img width="450" alt="image" src="https://github.com/xpsa0421/Project-Monster-Hunter-World/assets/71711432/2cc0306e-0ac0-44dd-a117-8a914664cc17">|
|Non Combat - Patrol| Combat - Aerial Breath |
|<img width="450" alt="image" src="https://github.com/xpsa0421/Project-Monster-Hunter-World/assets/71711432/4c858497-164a-4192-b1cc-e2f9985d7db4"> |<img width="450" alt="image" src="https://github.com/xpsa0421/Project-Monster-Hunter-World/assets/71711432/5d897c4c-8738-4f23-b210-1859d8531478">
|Combat - Aerial Stomp| Combat - Aerial Charge |
|<img width="450" alt="image" src="https://github.com/xpsa0421/Project-Monster-Hunter-World/assets/71711432/14611e63-86d3-4052-b55e-860b15617e3c"> |<img width="450" alt="image" src="https://github.com/xpsa0421/Project-Monster-Hunter-World/assets/71711432/8ac70a59-6dbb-4f84-850d-6125a5d632fb"> | 
|Behaviour Tree|Server-Client Synchronisation|
|<img width="450" alt="image" src="https://github.com/xpsa0421/Project-Monster-Hunter-World/assets/71711432/0007b3a5-eda2-40e4-8851-47f03ac60d5c">|<img width="450" alt="image" src="https://github.com/xpsa0421/Project-Monster-Hunter-World/assets/71711432/eddf4e6b-4d67-44e7-b521-09d3e95173a5">|








