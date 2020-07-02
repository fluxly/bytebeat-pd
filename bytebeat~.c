// megrimm 2011

#include "m_pd.h"
#include "bytebeat~.h"
#include <math.h>

static t_class *bytebeat_class;
static float a = 0.5;
static int formula = 100;

typedef struct _bytebeat {
    t_object x_obj;
    t_float x_f;
	t_int x_formula;
} t_bytebeat;

static void *bytebeat_new(void) {
    t_bytebeat *x = (t_bytebeat *)pd_new(bytebeat_class);
	outlet_new(&x->x_obj, gensym("signal"));
    x->x_f = 0;
	x->x_formula = 100;
    return (x);
}

static t_int *bytebeat_perform(t_int *w) {
	t_bytebeat *x = (t_bytebeat *)(w[1]);
	t_float *in = (t_float *)(w[2]);
    t_float *out = (t_float *)(w[3]);
    t_int n = (t_int)(w[4]);

    int t;
	
    while (n--) {
		t = *in++;
		//*out++ = (((t&0xff)^0x80)-128)/128; //putchar, conversion 2 unsigned char
		switch (x->x_formula) {
			case 100:  *out++ = t&(t>>4)>>3&t>>7; break;
			case 101:  *out++ = (((((t>>12)^(t>>12)-2)%11*t)/4|t>>13)&127); break; 
			case 102:  *out++ = ((t*("36364689"[t>>13&7]&15))/12&128); break; 
			case 103:  *out++ = (t>>5)|(t>>4)|((t%42)*(t>>4)|(0x15483113)-(t>>4))/(t>>16)^(t|(t>>4)); break; 
			case 104:  *out++ = ((t*5/53)|t*5+(t<<1)); break; 
			case 105:  *out++ = (t<65536)?((2*t*(t>>11)&(t-1)|(t>>4)-1)%64):(((t%98304)>65536)?((17*t*(t*t>>8)&(t-1)|(t>>7)-1)%128|(t>>4)):((13*t*(2*t>>16)&(t-1)|(t>>8)-1)%32|(t>>4))); break; 
			case 106:  *out++ = t>>16|((t>>4)%16)|((t>>4)%192)|(t*t%64)|(t*t%96)|(t>>16)*(t|t>>5); break; 
			case 107:  *out++ = t>>6^t&37|t+(t^t>>11)-t*((t%24?2:6)&t>>11)^t<<1&(t&598?t>>4:t>>10); break; 
			case 108:  *out++ = ((t/2*(15&(0x234568a0>>(t>>8&28))))|t/2>>(t>>11)^t>>12)+(t/16&t&24); break; 
			case 109:  *out++ = (t<65536)?((2*t*(t>>11)&(t-1)|(t>>4)-1)%64):(((t%98304)>65536)?((17*t*(2*t>>8)&(t-1)|(t>>6)-1)%64|(t>>4)):((15*t*(2*t>>16)&(t-1)|(t>>8)-1)%64|(t>>4))); break; 
			case 110:  *out++ = ((t>>4)*(13&(0x8898a989>>(t>>11&30)))&255)+((((t>>9|(t>>2)|t>>8)*10+4*((t>>2)&t>>15|t>>8))&255)>>1); break; 
			case 111:  *out++ = t*(((t>>12)|(t>>8))&(63&(t>>4))); break; 
			case 112:  *out++ = (t*(t>>5|t>>8))>>(t>>16); break; 
			case 113:  *out++ = t*(((t>>9)|(t>>13))&(25&(t>>6))); break; 
			case 114:  *out++ = t*(((t>>11)&(t>>8))&(123&(t>>3))); break; 
			case 115:  *out++ = t*(t>>8*((t>>15)|(t>>8))&(20|(t>>19)*5>>t|(t>>3))); break; 
			case 116:  *out++ = (t*t/256)&(t>>((t/1024)%16))^t%64*(0xC0D3DE4D69>>(t>>9&30)&t%32)*t>>18; break; 
			case 117:  *out++ = t*(t>>((t>>9)|(t>>8))&(63&(t>>4))); break; 
			case 118:  *out++ = (t>>6|t|t>>(t>>16))*10+((t>>11)&7); break; 
			case 119:  *out++ = (t%25-(t>>2|t*15|t%227)-t>>3)|((t>>5)&(t<<5)*1663|(t>>3)%1544)/(t%17|t%2048); break; 
			case 120:  *out++ = (t|(t>>9|t>>7))*t&(t>>11|t>>9); break; 
			case 121:  *out++ = t*5&(t>>7)|t*3&(t*4>>10); break; 
			case 122:  *out++ = (t>>7|t|t>>6)*10+4*(t&t>>13|t>>6); break; 
			case 123:  *out++ = ((t&4096)?((t*(t^t%255)|(t>>4))>>1):(t>>3)|((t&8192)?t<<2:t)); break; 
			case 124:  *out++ = ((t*(t>>8|t>>9)&46&t>>8))^(t&t>>13|t>>6); break; 
			case 125:  *out++ = (t*5&t>>7)|(t*3&t>>10); break; 
			case 126:  *out++ = (int)(t/1e7*t*t+t)%127|t>>4|t>>5|t%127+(t>>16)|t; break; 
			case 127:  *out++ = ((t/2*(15&(0x234568a0>>(t>>8&28))))|t/2>>(t>>11)^t>>12)+(t/16&t&24); break; 
			case 128:  *out++ = (t&t%255)-(t*3&t>>13&t>>6); break;
            case 129:  *out++ = (t&t%255)-(t*3&t>>13&t>>6); break;
			//case 129:  *out++ = t>>4|t&((t>>5)/(t>>7-(t>>15)&-t>>7-(t>>15))); break;
			case 130:  *out++ = ((t*("36364689"[t>>13&7]&15))/12&128)+(((((t>>12)^(t>>12)-2)%11*t)/4|t>>13)&127); break; 
			case 131:  *out++ = (t*9&t>>4|t*5&t>>7|t*3&t/1024)-1; break;  
			case 132:  *out++ = ((t*(t>>12)&(201*t/100)&(199*t/100))&(t*(t>>14)&(t*301/100)&(t*399/100)))+((t*(t>>16)&(t*202/100)&(t*198/100))-(t*(t>>17)&(t*302/100)&(t*298/100))); break; 
			case 133:  *out++ = ((t*(t>>12)&(201*t/100)&(199*t/100))&(t*(t>>14)&(t*301/100)&(t*399/100)))+((t*(t>>16)&(t*202/100)&(t*198/100))-(t*(t>>18)&(t*302/100)&(t*298/100))); break; 
			case 134:  *out++ = ((t*("36364689"[t>>13&7]&15))/12&128)+(((((t>>12)^(t>>12)-2)%11*t)/4|t>>13)&127); break; 
			case 135:  *out++ = t*(t^t+(t>>15|1)^(t-1280^t)>>10); break; 
			case 136:  *out++ = ((1-(((t+10)>>((t>>9)&((t>>14))))&(t>>4&-2)))*2)*(((t>>10)^((t+((t>>6)&127))>>10))&1)*32+128; break; 
			case 137:  *out++ = ((t>>1%128)+20)*3*t>>14*t>>18; break; 
			case 138:  *out++ = t*(((t>>9)&10)|((t>>11)&24)^((t>>10)&15&(t>>15))); break; 
			case 139:  *out++ = (t*t/256)&(t>>((t/1024)%16))^t%64*(0xC0D3DE4D69>>(t>>9&30)&t%32)*t>>18; break; 
			case 140:  *out++ = t&t>>8; break; 
			case 141:  *out++ = t*(42&t>>10); break; 
			case 142:  *out++ = t|t%255|t%257; break; 
			case 143:  *out++ = t>>6&1?t>>5:-t>>4; break; 
			case 144:  *out++ = t*(t>>9|t>>13)&16; break; 
			case 145:  *out++ = (t&t>>12)*(t>>4|t>>8); break; 
			case 146:  *out++ = (t*5&t>>7)|(t*3&t>>10); break; 
			case 147:  *out++ = (t*(t>>5|t>>8))>>(t>>16); break; 
			case 148:  *out++ = t*5&(t>>7)|t*3&(t*4>>10); break; 
			case 149:  *out++ = (t>>13|t%24)&(t>>7|t%19); break; 
			case 150:  *out++ = (t*((t>>9|t>>13)&15))&129; break; 
			case 151:  *out++ = (t&t%255)-(t*3&t>>13&t>>6); break; 
			case 152:  *out++ = (t&t>>12)*(t>>4|t>>8)^t>>6; break; 
			case 153:  *out++ = t*(((t>>9)^((t>>9)-1)^1)%13); break; 
			case 154:  *out++ = t*(0xCA98>>(t>>9&14)&15)|t>>8; break; 
			case 155:  *out++ = (t/8)>>(t>>9)*t/((t>>14&3)+4); break; 
			case 156:  *out++ = (~t/100|(t*3))^(t*3&(t>>5))&t; break; 
			case 157:  *out++ = (t|(t>>9|t>>7))*t&(t>>11|t>>9); break; 
			case 158:  *out++ = ((t>>1%128)+20)*3*t>>14*t>>18; break; 
			case 159:  *out++ = ((t&4096)?((t*(t^t%255)|(t>>4))>>1):(t>>3)|((t&8192)?t<<2:t)); break; 
			case 160:  *out++ = t*(((t>>12)|(t>>8))&(63&(t>>4))); break; 
			case 161:  *out++ = t*(((t>>9)|(t>>13))&(25&(t>>6))); break; 
			case 162:  *out++ = t*(t^t+(t>>15|1)^(t-1280^t)>>10); break; 
			case 163:  *out++ = t*(((t>>11)&(t>>8))&(123&(t>>3))); break; 
			case 164:  *out++ = (t>>7|t|t>>6)*10+4*(t&t>>13|t>>6); break; 
			case 165:  *out++ = (t*9&t>>4|t*5&t>>7|t*3&t/1024)-1; break; 
			case 166:  *out++ = t*(t>>((t>>9)|(t>>8))&(63&(t>>4))); break; 
			case 167:  *out++ = (t>>6|t|t>>(t>>16))*10+((t>>11)&7); break; 
			case 168:  *out++ = (t>>1)*(0xbad2dea1>>(t>>13)&3)|t>>5; break; 
			case 169:  *out++ = (t>>4)*(13&(0x8898a989>>(t>>11&30))); break; 
			case 170:  *out++ = (t>>(t&7))|(t<<(t&42))|(t>>7)|(t<<5); break; 
			case 171:  *out++ = (t>>7|t%45)&(t>>8|t%35)&(t>>11|t%20); break; 
			case 172:  *out++ = (t>>6|t<<1)+(t>>5|t<<3|t>>3)|t>>2|t<<1; break; 
			case 173:  *out++ = t+(t&t^t>>6)-t*((t>>9)&(t%16?2:6)&t>>9); break; 
			case 174:  *out++ = ((t*(t>>8|t>>9)&46&t>>8))^(t&t>>13|t>>6); break; 
			case 175:  *out++ = t*(((t>>9)^((t>>9)-1)^1)%13); break; 
			case 176:  *out++ = (t>>5)|(t<<4)|((t&1023)^1981)|((t-67)>>4); break;
            case 177:  *out++ = (t>>5)|(t<<4)|((t&1023)^1981)|((t-67)>>4); break;
			//case 177:  *out++ = t>>4|t&(t>>5)/(t>>7-(t>>15)&-t>>7-(t>>15)); break;
			case 178:  *out++ = t*(t/256)-t*(t/255)+t*(t>>5|t>>6|t<<2&t>>1); break; 
			case 179:  *out++ = ((t>>5&t)-(t>>5)+(t>>5&t))+(t*((t>>14)&14)); break; 
			case 180:  *out++ = (t*((3+(1^t>>10&5))*(5+(3&t>>14))))>>(t>>8&3); break;  
			case 181:  *out++ = ((t>>4)*(13&(0x8898a989>>(t>>11&30)))&255)+((((t>>9|(t>>2)|t>>8)*10+4*((t>>2)&t>>15|t>>8))&255)>>1); break; 
			case 182:  *out++ = (int)(t/1e7*t*t+t)%127|t>>4|t>>5|t%127+(t>>16)|t; break; 
			case 183:  *out++ = t*(((t>>9)&10)|((t>>11)&24)^((t>>10)&15&(t>>15))); break; 
			case 184:  *out++ = (~t>>2)*((127&t*(7&t>>10))<(245&t*(2+(5&t>>14)))); break; 
			case 185:  *out++ = (t+(t>>2)|(t>>5))+(t>>3)|((t>>13)|(t>>7)|(t>>11)); break; 
			case 186:  *out++ = t*(t>>8*((t>>15)|(t>>8))&(20|(t>>19)*5>>t|(t>>3))); break; 
			case 187:  *out++ = (t>>4)|(t%10)|(((t%101)|(t>>14))&((t>>7)|(t*t%17))); break; 
			case 188:  *out++ = ((t&((t>>5)))+(t|((t>>7))))&(t>>6)|(t>>5)&(t*(t>>7)); break; 
			case 189:  *out++ = ((t&((t>>23)))+(t|(t>>2)))&(t>>3)|(t>>5)&(t*(t>>7)); break; 
			case 190:  *out++ = (((((t*((t>>9|t>>13)&15))&255/15)*9)%(1<<7))<<2)%6<<4; break; 
			case 191:  *out++ = ((t%42)*(t>>4)|(0x15483113)-(t>>4))/(t>>16)^(t|(t>>4)); break; 
			case 192:  *out++ = t*(t>>((t&4096)?((t*t)/4096):(t/4096)))|(t<<(t/256))|(t>>4); break; 
			case 193:  *out++ = ((t&4096)?((t*(t^t%255)|(t>>4))>>1):(t>>3)|((t&8192)?t<<2:t)); break; 
			case 194:  *out++ = t*((0xbadbea75>>((t>>12)&30)&3)*0.25*(0x5afe5>>((t>>16)&28)&3)); break;
        
		}
	}
    return (w+5);
}

void bytebeat_dsp(t_bytebeat *x, t_signal **sp) {
    dsp_add(bytebeat_perform, 4, x, sp[0]->s_vec, sp[1]->s_vec, sp[0]->s_n);
}

static void bytebeat_changeFormula(t_bytebeat *x, t_floatarg f)
{
    x->x_formula = f + 100;
	post("formula: %d", x->x_formula);
	
}

void bytebeat_tilde_setup(void) {
    bytebeat_class = class_new(gensym("bytebeat~"), 
	    (t_newmethod)bytebeat_new, 0,
    	sizeof(t_bytebeat), 0, A_DEFFLOAT, 0);
    CLASS_MAINSIGNALIN(bytebeat_class, t_bytebeat, x_f);
    class_addmethod(bytebeat_class, (t_method)bytebeat_dsp, gensym("dsp"), 0);
	class_addmethod(bytebeat_class, (t_method)bytebeat_changeFormula, gensym("formula"), A_FLOAT, A_NULL);
	//addmess((method)bytebeat_changeFormula, "formula", A_INT32, 0);
	
    post("bytebeat~");
}
