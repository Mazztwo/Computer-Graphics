 #version 120

 attribute vec4 vPosition;
 attribute vec4 vNormal;
 varying vec4 color;

 uniform mat4 model_view_matrix;
 uniform mat4 projection_matrix;
 uniform mat4 ctm;

 uniform vec4 AmbientProduct, DiffuseProduct, SpecularProduct, LightPosition;
 uniform float shininess, attenuation_constant, attenuation_linear, attenuation_quadratic;
 uniform int isShadow;

 vec4 ambient, diffuse, specular;


vec4 calculateShadow(vec4 vertex, mat4 ctm)
{
    
    vec4 vPositionTemp = ctm * vertex;
    
    float x = (LightPosition.x)-((LightPosition.y)*(LightPosition.x - vPositionTemp.x)/(LightPosition.y - vPositionTemp.y));
    float y = 0.001;
    float z = (LightPosition.z)-((LightPosition.y)*(LightPosition.z - vPositionTemp.z)/(LightPosition.y - vPositionTemp.y));
    
    return vec4(x,y,z,1.0);
}



vec4 calculateColor()
{
    // Ambient
    ambient = AmbientProduct;
    
    // Diffuse
    vec4 N = normalize(model_view_matrix * ctm * vNormal);
    vec4 L_temp = model_view_matrix * (LightPosition - (ctm * vPosition));
    vec4 L = normalize(L_temp);
    diffuse = max(dot(L, N), 0.0) * DiffuseProduct;
    
    // Specular
    vec4 EyePoint = vec4(0.0, 0.0, 0.0, 1.0);
    vec4 V = normalize(EyePoint - (model_view_matrix * ctm * vPosition));
    vec4 H = normalize(L + V);
    specular = pow(max(dot(N, H), 0.0), shininess) * SpecularProduct;
    
    // Attenuation
    float distance = length(L_temp);
    float attenuation = 1/(attenuation_constant + (attenuation_linear * distance) + (attenuation_quadratic * distance * distance));
    
    return ambient + attenuation * (diffuse + specular);
    
    
}


 void main()
 {
     if(isShadow == 1)
     {
         color = vec4(0.0,0.0,0.0,1.0);
         gl_Position = projection_matrix * model_view_matrix * calculateShadow(vPosition, ctm);
     }
     else
     {
         color = calculateColor();
         gl_Position = projection_matrix * model_view_matrix * ctm * vPosition / vPosition.w;
         
     }
 }



