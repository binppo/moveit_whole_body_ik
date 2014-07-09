// Copyright  (C)  2007  Ruben Smits <ruben dot smits at mech dot kuleuven dot be>

// Version: 1.0
// Author: Ruben Smits <ruben dot smits at mech dot kuleuven dot be>
// Maintainer: Ruben Smits <ruben dot smits at mech dot kuleuven dot be>
// URL: http://www.orocos.org/kdl

// This library is free software; you can redistribute it and/or
// modify it under the terms of the GNU Lesser General Public
// License as published by the Free Software Foundation; either
// version 2.1 of the License, or (at your option) any later version.

// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
// Lesser General Public License for more details.

// You should have received a copy of the GNU Lesser General Public
// License along with this library; if not, write to the Free Software
// Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

#ifndef KDL_CHAIN_IKSOLVERVEL_PINV_NSO_HPP
#define KDL_CHAIN_IKSOLVERVEL_PINV_NSO_HPP

#include <stdlib.h>
#include "utilities/svd_HH.hpp"
#include "jacobian.hpp"

namespace KDL
{
/**
 * Implementation of a inverse velocity kinematics algorithm based
 * on the generalize pseudo inverse to calculate the velocity
 * transformation from Cartesian to joint space of a general
 * KDL::Chain. It uses a svd-calculation based on householders
 * rotations.
 *
 * **This uses the Gradient Projection Method to produce a gradient that causes self-motion 
 *   within the null space of the redudant kinematics (dof > 6).
 *
 * In case of a redundant robot this solver optimizes the the following criterium:
 * g=0.5*sum(weight*(Desired_joint_positions - actual_joint_positions))^2 as described in
 *  A. Liegeois. Automatic supervisory control of the configuration and
 * behavior of multibody mechnisms. IEEE Transactions on Systems, Man, and
 * Cybernetics, 7(12):868–871, 1977
 *
 * @ingroup KinematicFamily
 */
class IkSolverVel_pinv_nso
{
public:
  /**
   * Constructor of the solver
   *
   * @param chain the chain to calculate the inverse velocity
   * kinematics for
   * @param opt_pos the desired positions of the chain used by to resolve the redundancy
   * @param weights the weights applied in the joint space
   * @param eps if a singular value is below this value, its
   * inverse is set to zero, default: 0.00001
   * @param maxiter maximum iterations for the svd calculation,
   * default: 150
   * @param alpha the null-space velocity gain
   *
   */
  IkSolverVel_pinv_nso(int num_tips, int num_joints, JntArray joint_min, JntArray joint_max, JntArray weights, const Jacobian2d& jacobian,
    double eps=0.00001,int maxiter=150, double alpha = 0.25, bool verbose = false);

  ~IkSolverVel_pinv_nso() {};

  virtual int CartToJnt(const JntArray& q_in, const JntArray& xdot_in, Jacobian2d& jacobian, JntArray& qdot_out, JntArray& prev_H);

  /**
   *Set joint weights for optimization criterion
   *
   *@param weights the joint weights
   *
   */
  virtual int setWeights(const JntArray &weights);

  int setAllWeights(const double &weight);

  /**
   *Set null psace velocity gain
   *
   *@param alpha NUllspace velocity cgain
   *
   */
  virtual int setAlpha(const double alpha);

  void print(Eigen::MatrixXd &data) const;

private:
  //  Jacobian2d jacobian;
  SVD_HH svd;
  std::vector<JntArray> U;
  JntArray S;
  std::vector<JntArray> V;
  JntArray H; // performance criterion
  JntArray tmp; 
  JntArray tmp2;
  double eps;
  int maxiter;

  double alpha;
  JntArray weights;
  JntArray W; // weighting matrix
  JntArray joint_min;
  JntArray joint_max;
  JntArray joint_mid;
  JntArray joint_constant; // pre-compute some of the necessary values
  int num_tips; // number of end effectors to solve for
  bool verbose; // to show output debug info or not

  Eigen::MatrixXd pinverse_; // psuedo inverse matrix
};
}
#endif

