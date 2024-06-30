#define LIGHT_MODE_BLINN  0x01
#define LIGHT_MODE_PHONG  0x02

#define GAMMA_ON 0x01
#define GAMMA_OFF 0x02

#define SHADOW_ON = 0x01
#define SHADOW_OFF = 0x02

uniform int lightMode;
uniform int gamma;
uniform int shadow;