#define NUM_OPENGL_LIGHTS 8

//#include <string.h>
#include <iostream>
#include <fstream>
#include <string>
#include <GL/glui.h>
#include <GL/glut.h>
#include "Shape.h"
#include "Cube.h"
#include "Cylinder.h"
#include "Cone.h"
#include "Sphere.h"
//#include "SceneParser.h"
#include "phaseReader.h"
#include "Camera.h"

using namespace std;

/** These are the live variables passed into GLUI ***/
int  wireframe = 0;
int  fillObj = 1;
int velocity = 0;
int perspective = 0;
int expand = 0;
int freeze = 0;
int  segmentsX = 20;
int  segmentsY = 20;
int speed = 60;

int	 camRotU = 0;
int	 camRotV = 0;
int	 camRotW = 0;
int  viewAngle = 45;
float eyeX = 4;
float eyeY = 4;
float eyeZ = 4;
float lookX = -2;
float lookY = -2;
float lookZ = -2;

int time = 0;
int timerid = 0;
/** These are GLUI control panel objects ***/
int  main_window;
string filenamePath = "NVE.gro";
GLUI_EditText* filenameTextField = NULL;


/** these are the global variables used for rendering **/
Cube* cube = new Cube();
Cylinder* cylinder = new Cylinder();
Cone* cone = new Cone();
Sphere* sphere = new Sphere();
Shape* shape = NULL;
//SceneParser* parser = NULL;
Camera* camera = new Camera();
PhaseReader reader;

void setupCamera();

void callback_load(int id) {
	//char curDirName [2048];
	//if (filenameTextField == NULL) {
	//	return;
	//}
	//printf ("%s\n", filenameTextField->get_text());

	
	//parser = new SceneParser (filenamePath);

    reader.readIn(filenamePath);
	int empty = reader.isEmpty();

	cout << "success? " << !reader.isEmpty() << endl;
	if (empty == 1) {
		
	}
	else {
		setupCamera();
	}
}

void renderShape (int shapeType) {
	switch (shapeType) {
	case SHAPE_CUBE:
		shape = cube;
		break;
	case SHAPE_CYLINDER:
		shape = cylinder;
		break;
	case SHAPE_CONE:
		shape = cone;
		break;
	case SHAPE_SPHERE:
		shape = sphere;
		break;
	case SHAPE_SPECIAL1:
		shape = cube;
		break;
	default:
		shape = cube;
	}
	shape->setSegments(segmentsX, segmentsY);
	shape->draw();
}

/***************************************** myGlutIdle() ***********/

void myGlutIdle(void)
{
	/* According to the GLUT specification, the current window is
	undefined during an idle callback.  So we need to explicitly change
	it if necessary */
	if (glutGetWindow() != main_window)
		glutSetWindow(main_window);

	glutPostRedisplay();
}


/**************************************** myGlutReshape() *************/

void myGlutReshape(int x, int y)
{
	float xy_aspect;

	xy_aspect = (float)x / (float)y;
	glViewport(0, 0, x, y);

	camera->SetScreenSize(x, y);

	glutPostRedisplay();
}


/***************************************** setupCamera() *****************/
void setupCamera()
{
	SceneCameraData cameraData;
	//parser->getCameraData(cameraData);

	camera->Reset();  //note that this is a new function. Be sure to set values for near and far plane!
	camera->SetViewAngle(viewAngle);

	Point eye = Point(eyeX, eyeY, eyeZ);
	Vector focus = Vector(lookX, lookY, lookZ);

	Vector up = Vector(0, 1, 0);
	camera->Orient(eye, focus, up);
	camera->RotateV(camRotV);
	camera->RotateU(camRotU);
	camera->RotateW(camRotW);

	viewAngle = camera->GetViewAngle();
	Point eyeP = camera->GetEyePoint();
	Vector lookV = camera->GetLookVector();
	eyeX = eyeP[0];
	eyeY = eyeP[1];
	eyeZ = eyeP[2];
	lookX = lookV[0];
	lookY = lookV[1];
	lookZ = lookV[2];
	camRotU = 0;
	camRotV = 0;
	camRotW = 0;
	GLUI_Master.sync_live_all();
}



/***************************************** setLight() *****************/
void setLight(const SceneLightData &light)
{
    // Zero ambient light because the ambient light for the scene is
    // set with GL_LIGHT_MODEL_AMBIENT
    float zero[] = { 0, 0, 0, 0 };

    // There are 8 lights in OpenGL, GL_LIGHT0 to GL_LIGHT7, and
    // each GL_LIGHT* is one greater than the previous one.  This
    // means the nth light is GL_LIGHT0 + n.
    int id = GL_LIGHT0 + light.id;

    glLightfv(id, GL_AMBIENT, zero);
    glLightfv(id, GL_DIFFUSE, &light.color.r);
    glLightfv(id, GL_SPECULAR, &light.color.r);
    glLightf(id, GL_CONSTANT_ATTENUATION, light.function[0]);
    glLightf(id, GL_LINEAR_ATTENUATION, light.function[1]);
    glLightf(id, GL_QUADRATIC_ATTENUATION, light.function[2]);

    if (glIsEnabled(id))
        cout << "warning: GL_LIGHT" << light.id << " enabled more than once, not supposed to happen" << endl;

    switch (light.type)
    {
        case LIGHT_POINT:
        {
            // Convert from double[] to float[] and make sure the w coordinate is correct 
            float position[] = { (float)light.pos[0], (float)light.pos[1], (float)light.pos[2], 1 };
            glLightfv(id, GL_POSITION, position);
			glEnable(id);
            break;
        }

        case LIGHT_DIRECTIONAL:
        {
            // Convert from double[] to float[] and make sure the direction vector is normalized (it isn't for a lot of scene files)
            Vector direction = -light.dir;
			direction.normalize();
            float position[] = { (float)direction[0], (float)direction[1], (float)direction[2], (float)direction[3] };
            glLightfv(id, GL_POSITION, position);
			glEnable(id);
            break;
        }
    }
}


/***************************************** applyMaterial() *****************/
void applyMaterial(const SceneMaterial &material)
{
	SceneGlobalData globalData;
	//parser->getGlobalData(globalData);

	SceneMaterial material_local = material;
	material_local.cAmbient.r *= globalData.ka;
	material_local.cAmbient.g *= globalData.ka;
	material_local.cAmbient.b *= globalData.ka;
	material_local.cDiffuse.r *= globalData.kd;
	material_local.cDiffuse.g *= globalData.kd;
	material_local.cDiffuse.b *= globalData.kd;
	material_local.cSpecular.r *= globalData.ks;
	material_local.cSpecular.g *= globalData.ks;
	material_local.cSpecular.b *= globalData.ks;
	material_local.cReflective.r *= globalData.ks;
	material_local.cReflective.g *= globalData.ks;
	material_local.cReflective.b *= globalData.ks;
	material_local.cTransparent.r *= globalData.kt;
	material_local.cTransparent.g *= globalData.kt;
	material_local.cTransparent.b *= globalData.kt;

    glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, &material_local.cAmbient.r);
    glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, &material_local.cDiffuse.r);
    glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, &material_local.cSpecular.r);
    glMaterialfv(GL_FRONT_AND_BACK, GL_EMISSION, &material_local.cEmissive.r);
}


/***************************************** myGlutDisplay() *****************/

void myGlutDisplay(void)
{
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


	if (reader.isEmpty() == 1) {
		return;
	}

	camera->SetViewAngle(viewAngle);
	glMatrixMode(GL_PROJECTION);
	Matrix projection = camera->GetProjectionMatrix();
	glLoadMatrixd(projection.unpack());

    vector<struct frame> frames = reader.getFrames();

    Point eye;
    Vector focus;

    int index = 400;
    if (expand) {
        index = 800;
    }
    if (perspective) {
        eye = Point(frames.at(reader.getFrameNum()).pos[index][0] / 3.47, 
                          frames.at(reader.getFrameNum()).pos[index][1] / 3.47, 
                          frames.at(reader.getFrameNum()).pos[index][2] / 3.47);
        focus = Vector(frames.at(reader.getFrameNum()).vel[index][0], 
                          frames.at(reader.getFrameNum()).vel[index][1], 
                          frames.at(reader.getFrameNum()).vel[index][2]);
    } else {
        eye = Point(eyeX, eyeY, eyeZ); 
        focus = Vector(lookX, lookY, lookZ);
    }

    Vector up = Vector(0, 1, 0);
	camera->Orient(eye, focus, up);
	camera->RotateV(camRotV);
	camera->RotateU(camRotU);
	camera->RotateW(camRotW);
	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixd(camera->GetModelViewMatrix().unpack());

	//disable all the lights, and then enable each one...
	for (int i = 0; i < NUM_OPENGL_LIGHTS; i++) {
		glDisable(GL_LIGHT0 + i);
	}

   
    vector<struct scene_object> object_vector = reader.getObjects();
	vector<struct scene_velocity> velocity_vector = reader.getVectors();
	//drawing the axes
	glEnable(GL_COLOR_MATERIAL);
    glDisable(GL_LIGHTING);
    if (!expand) {
	    glBegin(GL_LINES);
        glColor3f(1.0, 0.0, 0.0);
        glVertex3f(0, 0, 0); glVertex3f(1.0, 0, 0);
        glVertex3f(0, 1.0, 0); glVertex3f(1.0, 1.0, 0);
        glVertex3f(0, 0, 1.0); glVertex3f(1.0, 0, 1.0);
        glVertex3f(0, 1.0, 1.0); glVertex3f(1.0, 1.0, 1.0);

        glColor3f(0.0, 1.0, 0.0);
        glVertex3f(0, 0, 0); glVertex3f(0.0, 1.0, 0);
        glVertex3f(0, 0, 1.0); glVertex3f(0.0, 1.0, 1.0);
        glVertex3f(1.0, 0, 0); glVertex3f(1.0, 1.0, 0);
        glVertex3f(1.0, 0, 1.0); glVertex3f(1.0, 1.0, 1.0);

        glColor3f(0.0, 0.0, 1.0);
        glVertex3f(0, 0, 0); glVertex3f(0, 0, 1.0);
        glVertex3f(1.0, 0, 0); glVertex3f(1.0, 0, 1.0);
        glVertex3f(0, 1.0, 0); glVertex3f(0, 1.0, 1.0);
        glVertex3f(1.0, 1.0, 0); glVertex3f(1.0, 1.0, 1.0);
        glEnd();
    }

	glColor3f(1.0, 0.0, 0.0);
	glDisable(GL_COLOR_MATERIAL);
	//int numLights = parser->getNumLights();


	glEnable(GL_LIGHTING);
	if (fillObj == 1) {
		glEnable(GL_POLYGON_OFFSET_FILL);
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		//TODO: render the scene...
		//note that you should always applyMaterial first, then draw the geometry
        for (struct scene_object sceneObj : object_vector) {
            glPushMatrix();
            glMultMatrixd(sceneObj.object_transformation.unpack());
            applyMaterial(sceneObj.scenePrim.material);
            renderShape(sceneObj.scenePrim.type);
            glPopMatrix();
        }
	}
	if (velocity == 1)
	{
		glEnable(GL_COLOR_MATERIAL);
		glDisable(GL_LIGHTING);
		
		for (scene_velocity sceneVel : velocity_vector) {
			glBegin(GL_LINES);
			glColor3f(1.0, 0.0, 0.0);
			glVertex3f(sceneVel.begin[0], sceneVel.begin[1], sceneVel.begin[2]); 
			glVertex3f(sceneVel.begin[0]+sceneVel.thevector[0], sceneVel.begin[1] + sceneVel.thevector[1], sceneVel.begin[2] + sceneVel.thevector[2]);
			glEnd();
		}
		
		glEnable(GL_LIGHTING);
	}
	glDisable(GL_LIGHTING);
	
	camera->RotateV(-camRotV);
	camera->RotateU(-camRotU);
	camera->RotateW(-camRotW);

	glutSwapBuffers();
}

void onExit()
{
	delete cube;
	delete cylinder;
	delete cone;
	delete sphere;
	delete camera;
}

void timeupdate(int value)
{
	if (reader.isEmpty() == 1)
	{
		glutTimerFunc(1000/speed, timeupdate, 0);
		return;
	}
	//cout << "test " << endl;
	if (value == 0)
	{
		reader.update(velocity, speed, expand);
	}
	glutTimerFunc(1000 /speed, timeupdate,0);
}


/**************************************** main() ********************/

int main(int argc, char* argv[])
{
	atexit(onExit);

	

	/****************************************/
	/*   Initialize GLUT and create window  */
	/****************************************/

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_RGB | GLUT_DOUBLE | GLUT_DEPTH);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(500, 500);

	main_window = glutCreateWindow("COMP 175 Assignment 3");
	glutDisplayFunc(myGlutDisplay);
	glutReshapeFunc(myGlutReshape);

	glShadeModel (GL_SMOOTH);

	glEnable(GL_DEPTH_TEST);

	// Specular reflections will be off without this, since OpenGL calculates
	// specular highlights using an infinitely far away camera by default, not
	// the actual location of the camera
	glLightModeli(GL_LIGHT_MODEL_LOCAL_VIEWER, 1);

	// Show all ambient light for the entire scene (not one by default)
	GLfloat one[] = { 0.5f, 0.5f, 0.5f, 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, one);

	glPolygonOffset(1, 1);



	/****************************************/
	/*         Here's the GLUI code         */
	/****************************************/

	GLUI* glui = GLUI_Master.create_glui("GLUI");

	filenameTextField = new GLUI_EditText( glui, "Filename:", filenamePath);
	filenameTextField->set_w(300);
	glui->add_button("Load", 0, callback_load);
	
	GLUI_Panel *camera_panel = glui->add_panel("Camera");
	(new GLUI_Spinner(camera_panel, "RotateV:", &camRotV))
		->set_int_limits(-179, 179);
	(new GLUI_Spinner(camera_panel, "RotateU:", &camRotU))
		->set_int_limits(-179, 179);
	(new GLUI_Spinner(camera_panel, "RotateW:", &camRotW))
		->set_int_limits(-179, 179);
	(new GLUI_Spinner(camera_panel, "Angle:", &viewAngle))
		->set_int_limits(1, 179);

	glui->add_column_to_panel(camera_panel, true);

	GLUI_Spinner* eyex_widget = glui->add_spinner_to_panel(camera_panel, "EyeX:", GLUI_SPINNER_FLOAT, &eyeX);
	eyex_widget->set_float_limits(-10, 10);
	GLUI_Spinner* eyey_widget = glui->add_spinner_to_panel(camera_panel, "EyeY:", GLUI_SPINNER_FLOAT, &eyeY);
	eyey_widget->set_float_limits(-10, 10);
	GLUI_Spinner* eyez_widget = glui->add_spinner_to_panel(camera_panel, "EyeZ:", GLUI_SPINNER_FLOAT, &eyeZ);
	eyez_widget->set_float_limits(-10, 10);

	GLUI_Spinner* lookx_widget = glui->add_spinner_to_panel(camera_panel, "LookX:", GLUI_SPINNER_FLOAT, &lookX);
	lookx_widget->set_float_limits(-10, 10);
	GLUI_Spinner* looky_widget = glui->add_spinner_to_panel(camera_panel, "LookY:", GLUI_SPINNER_FLOAT, &lookY);
	looky_widget->set_float_limits(-10, 10);
	GLUI_Spinner* lookz_widget = glui->add_spinner_to_panel(camera_panel, "LookZ:", GLUI_SPINNER_FLOAT, &lookZ);
	lookz_widget->set_float_limits(-10, 10);

	glui->add_column(true);

	GLUI_Panel *render_panel = glui->add_panel("Render");
	new GLUI_Checkbox(render_panel, "Fill", &fillObj);
	new GLUI_Checkbox(render_panel, "Velocity", &velocity);
	new GLUI_Checkbox(render_panel, "Perspective", &perspective);
	new GLUI_Checkbox(render_panel, "Expand", &expand);
	new GLUI_Checkbox(render_panel, "Freeze", &freeze);
	(new GLUI_Spinner(render_panel, "Segments X:", &segmentsX))
		->set_int_limits(3, 60);
	(new GLUI_Spinner(render_panel, "Segments Y:", &segmentsY))
		->set_int_limits(3, 60);
	(new GLUI_Spinner(render_panel, "Speed:", &speed))
		->set_int_limits(2, 60);


	glui->add_button("Quit", 0, (GLUI_Update_CB)exit);
	glutTimerFunc(1000 / speed, timeupdate, 0);

	glui->set_main_gfx_window(main_window);

	/* We register the idle callback with GLUI, *not* with GLUT */
	GLUI_Master.set_glutIdleFunc(myGlutIdle);

	glutMainLoop();

	return EXIT_SUCCESS;
}
