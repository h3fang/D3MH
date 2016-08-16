#ifndef D3MH_LEVELAREA_H
#define D3MH_LEVELAREA_H

#include "types.h"

#include "marker.h"
#include "scene.h"

namespace D3 {

// SizeOf = 0x680
struct LevelArea {
    ListB<void*>* x000_PtrListB_AttachmentList;
    Map<int, Marker>* x004_PtrEtcMarkersMap;
    Map<int, Marker>* x008_PtrQuestMarkersMap;
    int x00C_AnimationFrame;
    ListPack<SceneRevealInfo> x010_ListPack_Allocator_10x68Bytes_RevealedScenes;
    float x040_LabelOpacity;
    uint x044_SnoId;
    int x048_LoadingSnoId;
    int x04C_LabelState; // 0 = Not visible, 1 = Partially visible, 2 = Fully visible
    int x050;
    int x054_Neg1;
    int x058_IsMinimapVisible;
    int x05C;
    int x060_Flags;
    int _x064;
    UIReference x068_UIReference_MinimapDialog;
    UIReference x270_UIReference_MinimapMain;
    UIReference x478_UIReference_MinimapArrowRegion;
};

}

#endif // D3MH_LEVELAREA_H

