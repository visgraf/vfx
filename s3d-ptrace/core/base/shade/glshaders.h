
// phong vertex shader

const char* phong_vertp = {
  "attribute vec4 vertpos;"
  "attribute vec3 vertnormal;"
  "attribute vec3 verttexc;"
  "attribute vec3 vertdu;"
  "attribute vec3 vertdv;"
  "void main() {"
    "gl_Position = ModelViewProjectionMatrix * vertpos;"
    "veye = eye - vec3(vertpos);"
    "vpos = vertpos;"
    "vnorm = vertnormal;"
    "vtexc = verttexc;"
    "vdu = vertdu;"
    "vdv = vertdv;"
  "}"
};


const char* render_context = {
  //
  // render context
  //
  "uniform  mat4 ModelViewProjectionMatrix;"
  "uniform  float time;"
  "uniform  vec3 eye;"
  //
  "varying  vec3 veye;"
  "varying  vec4 vpos;"
  "varying  vec3 vnorm;"
  "varying  vec3 vtexc;"
  "varying  vec3 vdu;"
  "varying  vec3 vdv;"
};


const char* light_support = {
  //
  // Global state
  //
  "uniform sampler2D color_tex;"
  "vec4 pos;"
  "vec3 normal;"
  "vec3 viewer;"
  //
  // Material
  //
  "uniform  vec3 matambient;"
  "uniform  vec3 matdiffuse;"
  "uniform  vec3 matspecular;"
  "uniform  vec3 mattransmit;"
  "uniform  float matshininess;"   
  //
  // light transport
  //
  "struct cone { vec3 o; vec3 d; float cosa; };"
  "bool dir_coupling(cone a, vec3 v) {"
    "return (dot(a.d, -1.0*v) > a.cosa);"
  "}"
  "bool point_coupling(cone a, cone b) {"
     "vec3 d = normalize(a.o - b.o);"
     "return dir_coupling(a, d) && dir_coupling(b, -1.0*d);"
  "}"
  "struct illum { vec3 col; vec3 dir; bool on; };"
};


const char* light_sources = {
  //
  // light sources
  //
  "illum amb_light(int i, cone recv) {"
    "illum il;"
    "il.on = false;"
    "return il;"
  "}"
  "illum dist_light(int i, cone recv) {"
    "illum il;"
    "vec3 light_dir = normalize(light[i].dir);"
     "if (dir_coupling(recv, -1.0*light_dir) == false) {"
       "il.on = false;"
     "} else {"
       "il.dir = light_dir;"
       "il.col = light[i].col;"
       "il.on = true;"
     "}"
     "return il;"
  "}"
  "illum light_transport(int i, cone recv) {"
    "illum il;"
    "if (ltype[i] == 0)"
       "il = amb_light(i, recv);"
    "else if (ltype[i] == 1)"
       "il = dist_light(i, recv);"
    "else "
       "il.on = false;"
    "return il;"
  "}"
};


const char* illum_models = {
  //
  // light models
  //
  "vec3 ambient() { "
    "vec3 c = vec3(0,0,0);"
    "for (int i=0; i<nlights; i++) {"
          "c += light[i].amb;" 
    "}"
    "return c;"
  "}"
  "vec3 diffuse() { "
    "vec3 c = vec3(0,0,0);"
    "cone receiver = cone(pos.xyz, normal, cos(1.570796));"
    "for (int i=0; i<nlights; i++) {"
       "illum il = light_transport(i, receiver);"
       "if (il.on) "
          "c += il.col * dot(il.dir, normal);"
    "}"
    "return c;"
  "}"
};


// phong fragment shaders

const char* phong_matte_fragp = {
  //
  // material - matte
  //
  "void main() {"
    "vec3 color;"
    "set_lights();"
    "pos = vpos;"
    "normal = normalize(vnorm);"
    "if (gl_FrontFacing) {"
       "color =  matambient * ambient();" 
       "color +=  matdiffuse * diffuse();"
    "} else {"
       "color =  vec3(0.1,0.1,0.1);"
    "}"
  "gl_FragColor =  vec4(color, 1.0);"
  "}"
};


// phong fragment shader - material plastic

const char* phong_map_fragp = {
  //
  // material - textured
  //
  "void main() {"
    "vec3 color;"
    "set_lights();"
    "pos = vpos;"
    "normal = normalize(vnorm);"
    "if (gl_FrontFacing) {"
        "color  = vec3(texture2D(color_tex, vtexc.xy)) * diffuse();"
    "} else {"
       "color =  vec3(0.2,0.2,0.2);"
    "}"
  "gl_FragColor = vec4(color, 1.0);"
  "}"
};

// phong fragment shader - material plastic

const char* phong_proc_fragp = {
  //
  // material - procedural
  //
  "vec3 proc_tex(vec2 tc) { "
    "vec3 c = vec3(tc.x,tc.y,1.0);"
    "return c;"
  "}"
  "void main() {"
    "vec3 color;"
    "set_lights();"
    "pos = vpos;"
    "normal = normalize(vnorm);"
    "if (gl_FrontFacing) {"
        "color  = vec3(proc_tex(vtexc.xy)) * diffuse();"
    "} else {"
       "color =  vec3(0.2,0.2,0.2);"
    "}"
  "gl_FragColor = vec4(color, 1.0);"
  "}"
};


