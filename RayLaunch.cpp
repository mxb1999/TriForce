#include "implSim.h"

using namespace std;
void trackRays()
{
  cout << "Tracking Rays" << endl;
  double x0[nrays];
  double z0[nrays];
  double kx0[nrays];
  double kz0[nrays];
  double phase_x[nrays];
  double pow_x[nrays];

  span(phase_x,beam_min_z, beam_max_z, nrays);
  for(int i = 0; i < nrays; i++)
  {
    kx0[i] = 1.0;
    kz0[i] = -0.1;
    pow_x[i] = exp(-1*pow(pow(phase_x[i]/sigma,2.0),4.0/2.0));
    cout << pow_x[i] << endl;
    phase_x[i] += offset;
  }
  int finalts[nrays][nbeams];
  beam = 0;
  cout << "BEAMNUM is" << beam << endl;
  span(z0, beam_min_z, beam_max_z, nrays);
  cout <<  scientific;

  for(int i = 0; i < nrays; i++)
  {
    x0[i] = xmin-(dt/courant_mult*c*0.5);
    z0[i] += offset-(dz/2)-(dt/courant_mult*c*0.5);
  }
  //plt::figure();
  for(int i = 0; i < nrays;i++)
  {
    raynum = i;
    //cout<< "URAY INTERP_____________________" << endl;
    double interpNum = interp(phase_x, pow_x, z0[i], nrays);
    uray[0] = uray_mult*interpNum;
    cout << "Interp: " << interpNum << endl;
    cout << z0[i] << endl;
  //  printf("%s%lf\n", "z0[i]", z0[i]);
    injected += uray[0];
    launch_ray_XZ(x0[i],z0[i],kx0[i],kz0[i]);
  //  printf("%s%d\n", "finalt: ", finalt);
    *(finalts[i] + beam) = finalt;
    //cout << kx0[i] << " || " << kz0[i] << '\n';

  }
  for(int i = 0; i < nrays; i++)
  {
    kx0[i] = 0.0;
    kz0[i] = 1.0;
    phase_x[i] -= offset;
  }
  beam = 1;
  cout << "BEAMNUM is" << beam << endl;
  span(x0, beam_min_z, beam_max_z, nrays);
  for(int i = 0; i < nrays;i++)
  {
    raynum = i;
    z0[i] = zmin-(dt/courant_mult*c*0.5);
    x0[i] += (dx/2)-(dt/courant_mult*c*0.5);
    //cout<< "URAY INTERP_____________________" << endl;
    double interpNum = interp(phase_x, pow_x, x0[i], nrays);
    uray[0] = uray_mult*interpNum;

  //  printf("%s%lf\n", "z0[i]", z0[i]);
    //cout << "Interp: " << interpNum << endl;
    injected += uray[0];
    launch_ray_XZ(x0[i],z0[i],kx0[i],kz0[i]);
  //  printf("%s%d\n", "finalt: ", finalt);
    *(finalts[i] + beam) = finalt;
    //cout << kx0[i] << " || " << kz0[i] << '\n';

  }
//  plt::show();
printf("%s\n", "Got past launching the rays");


}
void updateIntersections()
{
  cout << "Updating Rays Intersections" << endl;
  for(int i = 0; i < nx; i++)
  {
    for(int j = 0; j < nz; j++)
    {
      intersections[i][j] = 0;
    }
  }
  for(int i = 1; i < nx;i++)
  {
    for(int j = 1; j < nz; j++)
    {
      for(int m = 0; m < numstored; m++)
      {
        if(marked[i][j][m][0] == 0)
        {
          break;
        }else
        {
          for(int l = 0; l < numstored; l++)
          {
            if(marked[i][j][m][1] == 0 )
            {
              break;
            }else
            {
              intersections[i][j] += 1.0;
            }
          }
        }
      }
    }
  }
}
void launchRays()
{
  trackRays();
  updateIntersections();
}