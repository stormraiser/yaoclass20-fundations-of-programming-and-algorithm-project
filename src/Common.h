#pragma once

// Region size definitions
#define REGION_SCALE 4
#define CHUNK_SCALE 4
#define REGION_SCALE_FULL (REGION_SCALE + CHUNK_SCALE)
#define CHUNK_NUMBER (1 << REGION_SCALE)
#define CHUNK_SIZE (1 << CHUNK_SCALE)
#define REGION_SIZE (1 << REGION_SCALE_FULL)

#define IDLE_TIME_LIMIT 100

// type identifiers
#define TYPE_COUNT 2

#define TYPEID_VOID -1
#define TYPEID_UNSIGNED_CHAR 0
#define TYPEID_DOUBLE_PAIR 1

#define RULE_FAMILY_COUNT 5

#define FAMILYID_LIFE_LIKE 0
#define FAMILYID_GENERATIONS 1
#define FAMILYID_REACTION_DIFFUSION 2
#define FAMILYID_MARGOLUS 3
#define FAMILYID_TEST 4

#define RENDER_TIME_MULTIPLE 25

#define BCAST(type, var) ((Board<type>*)(var))
#define GCAST(type, var) ((Grid<type>*)(var))
#define RCAST(type, var) ((Rule<type>*)(var))
#define SCAST(type, var) ((Shader<type>*)(var))
