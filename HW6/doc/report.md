

# Homework 6 - Lights and Shading

## Basic

### 1.  实现Phong光照模型： 

   场景中绘制一个cube 自己写shader实现两种shading: Phong Shading 和 Gouraud Shading，并解释两种shading的实现原理合理设置视点、光照位置、光照颜色等参数，使光照效果明显显示 

 实现Phong光照模型主要分为环境光, 漫反射和镜面反射三部分

#### 环境光

因为环境光是一种对全局关照的模拟, 所以它是一个常量, 因此可以设置ambient因子和光照颜色的乘机得到

```
vec3 ambient = ambientFactor * lightColor;
```

#### 漫反射

根据**Lambert’s Cosine Law（朗伯余弦定律)**, 物体表面反射的能量与入射光和表面法向量方向的夹角有关, 而且和视角无关. 因此我们需要计算表面法向量和光源方向的夹角的余弦值.

```
vec3 norm = normalize(fNormal);
vec3 lightDir = normalize(lightPosition - fragPosition);
float diff = max(dot(norm, lightDir), 0.0);
vec3 diffuse = deffuseFactor * diff * lightColor;
```

#### 镜面反射

根据Snell's law我们可以得到理想反射模型, 但因为理想的镜面是不存在的. 因此我们根据经验公式来计算镜面反射, 即通过指数来控制角度下降对反射的影响.

```
vec3 viewDir = normalize(viewPosition - fragPosition);
vec3 reflectDir = reflect(-lightDir, norm);  
float spec = pow(max(dot(viewDir, reflectDir), 0.0), reflectDegree);
vec3 specular = specularFactor * spec * lightColor;  
```

#### 计算反射光颜色

最后通过与物体颜色相乘得到反射光的颜色, 至此phong关照模型完成

```
vec3 result = (ambient + diffuse + specular) * objectColor;
```

####  Phong Shading 和 Gouraud Shading

phong shading是在顶点着色计算片段的法向量, 然后根据插值得到片段上每一个点的法向量, 最后在片段着色器中通过phong关照模型得到物体的颜色

Gouraud shading是在顶点着色器通过phong光照模型计算顶点的颜色, 然后通过插值得到片段上每个点的颜色.

#### 效果

- phong shading

![1557214493581](.\phong_shading.png)

* gouraud shading

![1557214577002](.\gouraud_shading)

可以看出phong shading得到的光照更加真实, 尤其是高光效果. 而gouraud shading效果也不错, 不过因为是通过顶点插值得到的光照, 所以这里的高光被插值平铺到了上表面上, 产生了不真实感.

###  2. 使用GUI，使参数可调节，效果实时更改：

 GUI里可以切换两种shading 使用如进度条这样的控件，使ambient因子、diﬀuse因子、specular因子、反光度等参数可调节，光照效 果实时更改 

#### 添加gui

```c++
		if (ImGui::BeginMenuBar())
		{	
			
			if (ImGui::BeginMenu("mode"))
			{
				ImGui::Checkbox("Phong Shading", &isPhongShading);
				ImGui::Checkbox("Move Light", &isMoving);
				ImGui::EndMenu();
			}
			ImGui::EndMenuBar();
		}

		ImGui::SliderFloat("ambient", &ambientFactor, 0, 1);
		ImGui::SliderFloat("diffuse", &diffuseFactor, 0, 1);
		ImGui::SliderFloat("specular", &specularFactor, 0, 10);
		ImGui::SliderInt("shiny", &reflectDegree, 1, 100);
```

#### 实时渲染

在每次渲染循环中改变着色器中的对应变量, 实现实时光照变化

```c++
if (isPhongShading) {
			cubeProgram.use();
			cubeProgram.setVec3(objectColor, "objectColor");
			cubeProgram.setVec3(lightColor, "lightColor");
			cubeProgram.setVec3(lightPosition, "lightPosition");
			cubeProgram.setVec3(viewPosition, "viewPosition");
			cubeProgram.setFloat(ambientFactor, "ambientFactor");
			cubeProgram.setFloat(diffuseFactor, "diffuseFactor");
			cubeProgram.setFloat(specularFactor, "specularFactor");
			cubeProgram.setInt(reflectDegree, "reflectDegree");
}
	...
```



#### 效果

![1557214893795](.\gui)

## Bonus: 

当前光源为静止状态，尝试使光源在场景中来回移动，光照效果实时更改。

#### 实现

根据时间函数, 让光源绕着物体旋转

```
if (isMoving) {
  theta += deltaTime;
  lightPosition[0] = 2 * cos(theta);
  lightPosition[2] = 2 * sin(theta);
}
```

改变着色器中光源的位置

```c++
model = glm::translate(glm::mat4(1.0f), glm::vec3(lightPosition[0], lightPosition[1], lightPosition[2]));
lightProgram.setMat4(model, "model");
```

同时改变物体着色器中光源的位置

```c++
cubeProgram.setVec3(lightPosition, "lightPosition");
```

#### 效果![moving](.\moving.gif)