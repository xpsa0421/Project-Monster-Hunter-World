//============================================
// 매크로.
//============================================
#ifndef SAFE_DELETE
#define SAFE_DELETE(p)       { if(p) { delete (p);     (p)=NULL; } }
#endif

#ifndef SAFE_DELETE_ARRAY
#define SAFE_DELETE_ARRAY(p) { if(p) { delete[] (p);   (p)=NULL; } }
#endif

#ifndef SAFE_RELEASE
#define SAFE_RELEASE(p)      { if(p) { (p)->Release(); (p)=NULL; } }
#endif

// Collision trace channels defines
#define ECC_PLAYER ECollisionChannel::ECC_GameTraceChannel1
#define ECC_MONSTER ECollisionChannel::ECC_GameTraceChannel2
#define ECC_PLAYER_ATTACK ECollisionChannel::ECC_GameTraceChannel3
#define ECC_MONSTER_CAPSULE ECollisionChannel::ECC_GameTraceChannel4