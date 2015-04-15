#if _FABM_DIMENSION_COUNT_==0

! ---------------------
! 0D spatial context
! ---------------------

#define _LOCATION_
#define _LOCATION_DIMENSIONS_

#elif _FABM_DIMENSION_COUNT_==1

! ---------------------
! 1D spatial context
! ---------------------

#define _LOCATION_ i__
#define _LOCATION_DIMENSIONS_ :

#ifdef _FABM_DEPTH_DIMENSION_INDEX_
#  define _GLOBAL_VERTICAL_(it) it
#endif

#ifdef _FABM_VECTORIZED_DIMENSION_INDEX_
#  define _INTERIOR_FIXED_LOCATION_
#  define _GLOBAL_INTERIOR_(it) it
#endif

#elif _FABM_DIMENSION_COUNT_==2

! ---------------------
! 2D spatial context
! ---------------------

#define _LOCATION_ i__,j__
#define _LOCATION_DIMENSIONS_ :,:

#ifdef _FABM_DEPTH_DIMENSION_INDEX_
#  if _FABM_DEPTH_DIMENSION_INDEX_==1
#    define _HORIZONTAL_LOCATION_ j__
#    define _GLOBAL_VERTICAL_(it) it,j__
#  elif _FABM_DEPTH_DIMENSION_INDEX_==2
#    define _HORIZONTAL_LOCATION_ i__
#    define _GLOBAL_VERTICAL_(it) i__,it
#  endif
#  define _HORIZONTAL_LOCATION_DIMENSIONS_ :
#endif

#if _FABM_VECTORIZED_DIMENSION_INDEX_==1
#  define _INTERIOR_FIXED_LOCATION_ j__
#  define _GLOBAL_INTERIOR_(it) it,j__
#  if _FABM_DEPTH_DIMENSION_INDEX_==2
#    define _GLOBAL_HORIZONTAL_(it) it
#  endif
#elif _FABM_VECTORIZED_DIMENSION_INDEX_==2
#  define _INTERIOR_FIXED_LOCATION_ i__
#  define _GLOBAL_INTERIOR_(it) i__,it
#  if _FABM_DEPTH_DIMENSION_INDEX_==1
#    define _GLOBAL_HORIZONTAL_(it) it
#  endif
#endif

#elif _FABM_DIMENSION_COUNT_==3

! ---------------------
! 3D spatial context
! ---------------------

#define _LOCATION_ i__,j__,k__
#define _LOCATION_DIMENSIONS_ :,:,:

#ifdef _FABM_DEPTH_DIMENSION_INDEX_
#  if _FABM_DEPTH_DIMENSION_INDEX_==1
#    define _HORIZONTAL_LOCATION_ j__,k__
#    define _GLOBAL_VERTICAL_(it) it,j__,k__
#  elif _FABM_DEPTH_DIMENSION_INDEX_==2
#    define _HORIZONTAL_LOCATION_ i__,k__
#    define _GLOBAL_VERTICAL_(it) i__,it,k__
#  elif _FABM_DEPTH_DIMENSION_INDEX_==3
#    define _HORIZONTAL_LOCATION_ i__,j__
#    define _GLOBAL_VERTICAL_(it) i__,j__,it
#  endif
#  define _HORIZONTAL_LOCATION_DIMENSIONS_ :,:
#endif

#if _FABM_VECTORIZED_DIMENSION_INDEX_==1
#  define _INTERIOR_FIXED_LOCATION_ j__,k__
#  define _GLOBAL_INTERIOR_(it) it,j__,k__
#  if _FABM_DEPTH_DIMENSION_INDEX_==2
#    define _GLOBAL_HORIZONTAL_(it) it,k__
#  elif _FABM_DEPTH_DIMENSION_INDEX_==3
#    define _GLOBAL_HORIZONTAL_(it) it,j__
#  endif
#elif _FABM_VECTORIZED_DIMENSION_INDEX_==2
#  define _INTERIOR_FIXED_LOCATION_ i__,k__
#  define _GLOBAL_INTERIOR_(it) i__,it,k__
#  if _FABM_DEPTH_DIMENSION_INDEX_==1
#    define _GLOBAL_HORIZONTAL_(it) it,k__
#  elif _FABM_DEPTH_DIMENSION_INDEX_==3
#    define _GLOBAL_HORIZONTAL_(it) i__,it
#  endif
#elif _FABM_VECTORIZED_DIMENSION_INDEX_==3
#  define _INTERIOR_FIXED_LOCATION_ i__,j__
#  define _GLOBAL_INTERIOR_(it) i__,j__,it
#  if _FABM_DEPTH_DIMENSION_INDEX_==1
#    define _GLOBAL_HORIZONTAL_(it) j__,it
#  elif _FABM_DEPTH_DIMENSION_INDEX_==2
#    define _GLOBAL_HORIZONTAL_(it) i__,it
#  endif
#endif

#endif

#if _FABM_VECTORIZED_DIMENSION_INDEX_==1
#  define _ITERATOR_ i__
#elif _FABM_VECTORIZED_DIMENSION_INDEX_==2
#  define _ITERATOR_ j__
#elif _FABM_VECTORIZED_DIMENSION_INDEX_==3
#  define _ITERATOR_ k__
#endif

! If there is no depth dimension, horizontal dimensions match full dimensions.
#ifndef _FABM_DEPTH_DIMENSION_INDEX_
#  define _HORIZONTAL_LOCATION_ _LOCATION_
#  define _HORIZONTAL_LOCATION_DIMENSIONS_ _LOCATION_DIMENSIONS_
#  define _HORIZONTAL_DIMENSION_COUNT_ _FABM_DIMENSION_COUNT_
#else
#  define _HORIZONTAL_DIMENSION_COUNT_ _FABM_DIMENSION_COUNT_-1
#endif

#if defined(_FABM_VECTORIZED_DIMENSION_INDEX_)&&!defined(_FABM_DEPTH_DIMENSION_INDEX_)
#  define _GLOBAL_HORIZONTAL_(it) _GLOBAL_INTERIOR_(it)
#endif

! Check for additional required preprocessor variables.
#ifndef _LOCATION_
#  error BUG: Preprocessor variable _LOCATION_ must be defined.
#endif
#ifndef _LOCATION_DIMENSIONS_
#  error BUG: Preprocessor variable _LOCATION_DIMENSIONS_ must be defined.
#endif
#if _HORIZONTAL_DIMENSION_COUNT_>0
#  ifndef _HORIZONTAL_LOCATION_
#    error BUG: Preprocessor variable _HORIZONTAL_LOCATION_ must be defined.
#  endif
#  ifndef _LOCATION_DIMENSIONS_
#    error BUG: Preprocessor variable _HORIZONTAL_LOCATION_DIMENSIONS_ must be defined.
#  endif
#endif
#if defined(_FABM_VECTORIZED_DIMENSION_INDEX_)&&!defined(_GLOBAL_INTERIOR_)
#  error BUG: Preprocessor variable _GLOBAL_INTERIOR_ must be defined since _FABM_VECTORIZED_DIMENSION_INDEX_ is set.
#endif
#if defined(_FABM_DEPTH_DIMENSION_INDEX_)&&!defined(_GLOBAL_VERTICAL_)
#  error BUG: Preprocessor variable _GLOBAL_VERTICAL_ must be defined since _FABM_DEPTH_DIMENSION_INDEX_ is set.
#endif
#if defined(_FABM_VECTORIZED_DIMENSION_INDEX_)&&_FABM_VECTORIZED_DIMENSION_INDEX_!=_FABM_DEPTH_DIMENSION_INDEX_&&!defined(_GLOBAL_HORIZONTAL_)
#  error BUG: Preprocessor variable _GLOBAL_HORIZONTAL_ must be defined since _FABM_VECTORIZED_DIMENSION_INDEX_ is set and not equal to _FABM_DEPTH_DIMENSION_INDEX_.
#endif

! =======================================================================================================
! Process spatial mask, based on the following variables provided by the driver:
!   _FABM_MASK_TYPE_ (data type of mask elements, e.g., logical, integer or real)
!   _FABM_MASKED_VALUE_ or _FABM_UNMASKED_VALUE_ (mask value for masked and unmasked cells, respectively)
! =======================================================================================================

#ifdef _FABM_MASK_TYPE_
#  define _HAS_MASK_
#endif

#ifdef _HAS_MASK_
#  ifndef _INTERIOR_IS_VECTORIZED_
#    error _FABM_MASK_TYPE_/_FABM_MASKED_VALUE_/_FABM_UNMASKED_VALUE_ are not used if no dimension is vectorized.
#  endif
#  ifdef _FABM_MASKED_VALUE_
#    define _IS_UNMASKED_(maskvalue) maskvalue/=_FABM_MASKED_VALUE_
#  elif defined(_FABM_UNMASKED_VALUE_)
#    define _IS_UNMASKED_(maskvalue) maskvalue==_FABM_UNMASKED_VALUE_
#  else
#    error If _FABM_MASK_TYPE_ is set, _FABM_MASKED_VALUE_ or _FABM_UNMASKED_VALUE_ must be set as well.
#  endif
#else
#  ifdef _FABM_MASKED_VALUE_
#    error To use _FABM_MASKED_VALUE_, _FABM_MASK_TYPE_ must be set as well.
#  endif
#  ifdef _FABM_UNMASKED_VALUE_
#    error To use _FABM_UNMASKED_VALUE_, _FABM_MASK_TYPE_ must be set as well.
#  endif
#endif

! =================================================================================
! Further preprocessor macros for specifying spatial dimensionality and position
! =================================================================================

! ---------------------------------------------------------------------------------
! Dimension attribute and index specifyer for horizontal (2D) fields.
! ---------------------------------------------------------------------------------

#if _HORIZONTAL_DIMENSION_COUNT_>0
#  define _INDEX_HORIZONTAL_LOCATION_ (_HORIZONTAL_LOCATION_)
#  define _DIMENSION_GLOBAL_HORIZONTAL_ ,dimension(_HORIZONTAL_LOCATION_DIMENSIONS_)
#  define _PREARG_HORIZONTAL_LOCATION_ _HORIZONTAL_LOCATION_,
#  define _PREARG_HORIZONTAL_LOCATION_DIMENSIONS_ _HORIZONTAL_LOCATION_DIMENSIONS_,
#else
#  define _INDEX_HORIZONTAL_LOCATION_
#  define _DIMENSION_GLOBAL_HORIZONTAL_
#  define _PREARG_HORIZONTAL_LOCATION_
#  define _PREARG_HORIZONTAL_LOCATION_DIMENSIONS_
#endif

! ---------------------------------------------------------------------------------
! Dimension attribute and index specifyer for full 3D fields.
! ---------------------------------------------------------------------------------

#if _FABM_DIMENSION_COUNT_>0
#  define _INDEX_LOCATION_ (_LOCATION_)
#  define _DIMENSION_GLOBAL_ ,dimension(_LOCATION_DIMENSIONS_)
#  define _ARGUMENTS_LOCATION_ ,_LOCATION_
#  define _PREARG_LOCATION_ _LOCATION_,
#  define _PREARG_LOCATION_DIMENSIONS_ _LOCATION_DIMENSIONS_,
#  define _DECLARE_ARGUMENTS_LOCATION_ integer,intent(in) :: _LOCATION_
#else
#  define _INDEX_LOCATION_
#  define _DIMENSION_GLOBAL_
#  define _ARGUMENTS_LOCATION_
#  define _PREARG_LOCATION_
#  define _PREARG_LOCATION_DIMENSIONS_
#  define _DECLARE_ARGUMENTS_LOCATION_
#endif

#define _DIMENSION_GLOBAL_PLUS_1_ ,dimension(_PREARG_LOCATION_DIMENSIONS_ :)
#define _DIMENSION_GLOBAL_HORIZONTAL_PLUS_1_ ,dimension(_PREARG_HORIZONTAL_LOCATION_DIMENSIONS_ :)

#ifdef _GLOBAL_INTERIOR_
!  Interior is vectorized; forward provided iterator
#  define _INDEX_GLOBAL_INTERIOR_(it) (_GLOBAL_INTERIOR_(it))
#  define _INDEX_GLOBAL_INTERIOR_PLUS_1_(it,j) (_GLOBAL_INTERIOR_(it),j)
#else
!  Interior is not vectorized; just index to local point in space.
#  define _INDEX_GLOBAL_INTERIOR_(it) _INDEX_LOCATION_
#  define _INDEX_GLOBAL_INTERIOR_PLUS_1_(it,j) (_PREARG_LOCATION_ j)
#endif

#ifdef _GLOBAL_HORIZONTAL_
!  Interior is vectorized; forward provided iterator
#  define _INDEX_GLOBAL_HORIZONTAL_(it) (_GLOBAL_HORIZONTAL_(it))
#  define _INDEX_GLOBAL_HORIZONTAL_PLUS_1_(it,j) (_GLOBAL_HORIZONTAL_(it),j)
#else
!  Interior is not vectorized; just index to local point in space.
#  define _INDEX_GLOBAL_HORIZONTAL_(it) _INDEX_HORIZONTAL_LOCATION_
#  define _INDEX_GLOBAL_HORIZONTAL_PLUS_1_(it,j) (_PREARG_HORIZONTAL_LOCATION_ j)
#endif

#ifdef _GLOBAL_VERTICAL_
!  Interior is vectorized; forward provided iterator
#  define _INDEX_GLOBAL_VERTICAL_(it) (_GLOBAL_VERTICAL_(it))
#  define _INDEX_GLOBAL_VERTICAL_PLUS_1_(it,j) (_GLOBAL_VERTICAL_(it),j)
#else
!  Interior is not vectorized; just index to local point in space.
#  define _INDEX_GLOBAL_VERTICAL_(it) _INDEX_LOCATION_
#  define _INDEX_GLOBAL_VERTICAL_PLUS_1_(it,j) (_PREARG_LOCATION_ j)
#endif

#ifdef _FABM_VECTORIZED_DIMENSION_INDEX_
!  ---------------------------------------------------------------------------------
!  INTERIOR procedures operate on a data slice over one spatial dimension.
!  ---------------------------------------------------------------------------------
#  if _FABM_DIMENSION_COUNT_>1
#    define _ARG_INTERIOR_FIXED_LOCATION_ ,_INTERIOR_FIXED_LOCATION_
#  else
#    define _ARG_INTERIOR_FIXED_LOCATION_
#  endif
#  define _ARGUMENTS_INTERIOR_LENGTH_ ,_N_
#  define _ARGUMENTS_INTERIOR_IN_ ,loop_start,loop_stop _ARG_INTERIOR_FIXED_LOCATION_
#  define _DECLARE_ARGUMENTS_INTERIOR_IN_ integer,intent(in) :: loop_start,loop_stop _ARG_INTERIOR_FIXED_LOCATION_;integer :: _ITERATOR_
#else
!  ---------------------------------------------------------------------------------
!  INTERIOR procedures operate on one point at a time.
!  ---------------------------------------------------------------------------------
#  define _ARGUMENTS_INTERIOR_LENGTH_
#  define _ARGUMENTS_INTERIOR_IN_ _ARGUMENTS_LOCATION_
#  define _DECLARE_ARGUMENTS_INTERIOR_IN_ _DECLARE_ARGUMENTS_LOCATION_
#endif

#ifdef _HORIZONTAL_IS_VECTORIZED_
!  ---------------------------------------------------------------------------------
!  HORIZONTAL procedures operate on a data slice over one spatial dimension.
!  This will be the same dimension that INTERIOR procedures operate upon.
!  ---------------------------------------------------------------------------------
#  define _ARGUMENTS_HORIZONTAL_LENGTH_ _ARGUMENTS_INTERIOR_LENGTH_
#  define _ARGUMENTS_HORIZONTAL_IN_ _ARGUMENTS_INTERIOR_IN_
#  define _DECLARE_ARGUMENTS_HORIZONTAL_IN_ _DECLARE_ARGUMENTS_INTERIOR_IN_
#else
!  ---------------------------------------------------------------------------------
!  HORIZONTAL procedures operate on one point at a time.
!  ---------------------------------------------------------------------------------
#  define _ARGUMENTS_HORIZONTAL_LENGTH_
#  define _ARGUMENTS_HORIZONTAL_IN_ _ARGUMENTS_LOCATION_
#  define _DECLARE_ARGUMENTS_HORIZONTAL_IN_ _DECLARE_ARGUMENTS_LOCATION_
#endif

#ifdef _FABM_DEPTH_DIMENSION_INDEX_
!  ---------------------------------------------------------------------------------
!  VERTICAL procedures operate on a data slice over one spatial dimension.
!  ---------------------------------------------------------------------------------
#  if _FABM_DEPTH_DIMENSION_INDEX_==1
#    define _VERTICAL_ITERATOR_ i__
#  elif _FABM_DEPTH_DIMENSION_INDEX_==2
#    define _VERTICAL_ITERATOR_ j__
#  else
#    define _VERTICAL_ITERATOR_ k__
#  endif
#  if _FABM_DIMENSION_COUNT_==1
#    define _ARG_VERTICAL_FIXED_LOCATION_
#  else
#    define _ARG_VERTICAL_FIXED_LOCATION_ ,_HORIZONTAL_LOCATION_
#  endif
#  define _ARGUMENTS_VERTICAL_LENGTH_ ,_N_
#  define _ARGUMENTS_VERTICAL_IN_ ,loop_start,loop_stop _ARG_VERTICAL_FIXED_LOCATION_
#  define _DECLARE_ARGUMENTS_VERTICAL_IN_ integer,intent(in) :: loop_start,loop_stop _ARG_VERTICAL_FIXED_LOCATION_;integer :: _VERTICAL_ITERATOR_
#else
!  ---------------------------------------------------------------------------------
!  VERTICAL procedures operate on one point at a time.
!  ---------------------------------------------------------------------------------
#  define _ARGUMENTS_VERTICAL_LENGTH_
#  define _ARGUMENTS_VERTICAL_IN_ _ARGUMENTS_LOCATION_
#  define _DECLARE_ARGUMENTS_VERTICAL_IN_ _DECLARE_ARGUMENTS_LOCATION_
#endif

! For FABM: standard arguments used in calling biogeochemical routines.
#define _ARGUMENTS_SHARED_OUT_ self%environment
#define _ARGUMENTS_INTERIOR_OUT_ _ARGUMENTS_SHARED_OUT_ _ARGUMENTS_INTERIOR_LENGTH_
#define _ARGUMENTS_HORIZONTAL_OUT_ _ARGUMENTS_SHARED_OUT_ _ARGUMENTS_HORIZONTAL_LENGTH_
#define _ARGUMENTS_VERTICAL_OUT_ _ARGUMENTS_SHARED_OUT_ _ARGUMENTS_VERTICAL_LENGTH_

#ifdef _HAS_MASK_
#  define _PACK_GLOBAL_(in,out,i,mask) out(:,i) = pack(in _INDEX_GLOBAL_INTERIOR_(loop_start:loop_stop),mask)
#  define _UNPACK_(in,i,out,mask) out(:) = unpack(in(:,i),mask,0._rk)
#  define _UNPACK_TO_PLUS_1_(in,i,out,j,mask) out(:,j) = unpack(in(:,i),mask,0._rk)
#  define _UNPACK_AND_ADD_TO_PLUS_1_(in,i,out,j,mask) out(:,j) = out(:,j) + unpack(in(:,i),mask,0._rk)
#  define _UNPACK_TO_GLOBAL_(in,i,out,mask) out _INDEX_GLOBAL_INTERIOR_(loop_start:loop_stop) = unpack(in(:,i),mask,0._rk)
#  define _UNPACK_TO_GLOBAL_PLUS_1_(in,i,out,j,mask) out _INDEX_GLOBAL_INTERIOR_PLUS_1_(loop_start:loop_stop,j) = unpack(in(:,i),mask,0._rk)
#else
#  define _PACK_GLOBAL_(in,out,i,mask) _CONCURRENT_LOOP_BEGIN_;out _INDEX_SLICE_PLUS_1_(i) = in _INDEX_GLOBAL_INTERIOR_(loop_start+_I_-1);_LOOP_END_
#  define _UNPACK_(in,i,out,mask) _CONCURRENT_LOOP_BEGIN_;out _INDEX_EXT_SLICE_ = in _INDEX_SLICE_PLUS_1_(i);_LOOP_END_
#  define _UNPACK_TO_PLUS_1_(in,i,out,j,mask) _CONCURRENT_LOOP_BEGIN_;out _INDEX_EXT_SLICE_PLUS_1_(j) = in _INDEX_SLICE_PLUS_1_(i);_LOOP_END_
#  define _UNPACK_AND_ADD_TO_PLUS_1_(in,i,out,j,mask) _CONCURRENT_LOOP_BEGIN_;out _INDEX_EXT_SLICE_PLUS_1_(j) = out _INDEX_EXT_SLICE_PLUS_1_(j) + in _INDEX_SLICE_PLUS_1_(i);_LOOP_END_
#  define _UNPACK_TO_GLOBAL_(in,i,out,mask) _CONCURRENT_LOOP_BEGIN_;out _INDEX_GLOBAL_INTERIOR_(loop_start+_I_-1) = in _INDEX_SLICE_PLUS_1_(i);_LOOP_END_
#  define _UNPACK_TO_GLOBAL_PLUS_1_(in,i,out,j,mask) _CONCURRENT_LOOP_BEGIN_;out _INDEX_GLOBAL_INTERIOR_PLUS_1_(loop_start+_I_-1,j) = in _INDEX_SLICE_PLUS_1_(i);_LOOP_END_
#endif

#if defined(_HORIZONTAL_IS_VECTORIZED_)&&defined(_HAS_MASK_)
#  define _HORIZONTAL_PACK_GLOBAL_(in,out,j,mask) out(:,j) = pack(in _INDEX_GLOBAL_HORIZONTAL_(loop_start:loop_stop),mask)
#  define _HORIZONTAL_PACK_GLOBAL_PLUS_1_(in,i,out,j,mask) out(:,j) = pack(in _INDEX_GLOBAL_HORIZONTAL_PLUS_1_(loop_start:loop_stop,i),mask)
#  define _HORIZONTAL_UNPACK_TO_PLUS_1_(in,i,out,j,mask) out(:,j) = unpack(in(:,i),mask,0._rk)
#  define _HORIZONTAL_UNPACK_AND_ADD_TO_PLUS_1_(in,i,out,j,mask) out(:,j) = out(:,j) + unpack(in(:,i),mask,0._rk)
#  define _HORIZONTAL_UNPACK_TO_GLOBAL_(in,i,out,mask) out _INDEX_GLOBAL_HORIZONTAL_(loop_start:loop_stop) = unpack(in(:,i),mask,0._rk)
#  define _HORIZONTAL_UNPACK_TO_GLOBAL_PLUS_1_(in,i,out,j,mask) out _INDEX_GLOBAL_HORIZONTAL_PLUS_1_(loop_start:loop_stop,j) = unpack(in(:,i),mask,0._rk)
#else
#  define _HORIZONTAL_PACK_GLOBAL_(in,out,j,mask) _CONCURRENT_HORIZONTAL_LOOP_BEGIN_;out _INDEX_HORIZONTAL_SLICE_PLUS_1_(j) = in _INDEX_GLOBAL_HORIZONTAL_(loop_start+_J_-1);_HORIZONTAL_LOOP_END_
#  define _HORIZONTAL_PACK_GLOBAL_PLUS_1_(in,i,out,j,mask) _CONCURRENT_HORIZONTAL_LOOP_BEGIN_;out _INDEX_HORIZONTAL_SLICE_PLUS_1_(j) = in _INDEX_GLOBAL_HORIZONTAL_PLUS_1_(loop_start+_I_-1,i);_HORIZONTAL_LOOP_END_
#  define _HORIZONTAL_UNPACK_TO_PLUS_1_(in,i,out,j,mask) _CONCURRENT_HORIZONTAL_LOOP_BEGIN_;out _INDEX_HORIZONTAL_SLICE_PLUS_1_(j) = in _INDEX_HORIZONTAL_SLICE_PLUS_1_(i);_HORIZONTAL_LOOP_END_
#  define _HORIZONTAL_UNPACK_AND_ADD_TO_PLUS_1_(in,i,out,j,mask) _CONCURRENT_HORIZONTAL_LOOP_BEGIN_;out _INDEX_HORIZONTAL_SLICE_PLUS_1_(j) = out _INDEX_HORIZONTAL_SLICE_PLUS_1_(j) + in _INDEX_HORIZONTAL_SLICE_PLUS_1_(i);_HORIZONTAL_LOOP_END_
#  define _HORIZONTAL_UNPACK_TO_GLOBAL_(in,i,out,mask) _CONCURRENT_HORIZONTAL_LOOP_BEGIN_;out _INDEX_GLOBAL_HORIZONTAL_(loop_start+_J_-1) = in _INDEX_HORIZONTAL_SLICE_PLUS_1_(i);_HORIZONTAL_LOOP_END_
#  define _HORIZONTAL_UNPACK_TO_GLOBAL_PLUS_1_(in,i,out,j,mask) _CONCURRENT_HORIZONTAL_LOOP_BEGIN_;out _INDEX_GLOBAL_HORIZONTAL_PLUS_1_(loop_start+_J_-1,j) = in _INDEX_HORIZONTAL_SLICE_PLUS_1_(i);_HORIZONTAL_LOOP_END_
#endif

#if defined(_FABM_DEPTH_DIMENSION_INDEX_)&&defined(_HAS_MASK_)
#  define _VERTICAL_UNPACK_TO_GLOBAL_PLUS_1_(in,i,out,j,mask) out _INDEX_GLOBAL_VERTICAL_PLUS_1_(loop_start:loop_stop,j) = unpack(in(:,i),mask,0._rk)
#else
#  define _VERTICAL_UNPACK_TO_GLOBAL_PLUS_1_(in,i,out,j,mask) _CONCURRENT_VERTICAL_LOOP_BEGIN_;out _INDEX_GLOBAL_VERTICAL_PLUS_1_(loop_start+_I_-1,j) = in _INDEX_SLICE_PLUS_1_(i);_VERTICAL_LOOP_END_
#endif

#ifdef _FABM_VECTORIZED_DIMENSION_INDEX_
#  define _ARGUMENTS_SLICE_LENGTH_ ,_N_
#  define _DECLARE_ARGUMENTS_SLICE_LENGTH_ integer,intent(out) :: _N_
#  define _DECLARE_INTERIOR_ITERATORS_ integer :: _N_;_DECLARE_INTERIOR_INDICES_
#else
#  define _ARGUMENTS_SLICE_LENGTH_
#  define _DECLARE_ARGUMENTS_SLICE_LENGTH_
#  define _DECLARE_INTERIOR_ITERATORS_ _DECLARE_INTERIOR_INDICES_
#endif

#ifdef _HORIZONTAL_IS_VECTORIZED_
#  define _ARGUMENTS_HORIZONTAL_SLICE_LENGTH_ ,_N_
#  define _DECLARE_ARGUMENTS_HORIZONTAL_SLICE_LENGTH_ integer,intent(out) :: _N_
#  define _DECLARE_HORIZONTAL_ITERATORS_ integer :: _N_;_DECLARE_HORIZONTAL_INDICES_
#else
#  define _ARGUMENTS_HORIZONTAL_SLICE_LENGTH_
#  define _DECLARE_ARGUMENTS_HORIZONTAL_SLICE_LENGTH_
#  define _DECLARE_HORIZONTAL_ITERATORS_ _DECLARE_HORIZONTAL_INDICES_
#endif

#ifdef _FABM_DEPTH_DIMENSION_INDEX_
#  define _ARGUMENTS_VERTICAL_SLICE_LENGTH_ ,_N_
#  define _DECLARE_ARGUMENTS_VERTICAL_SLICE_LENGTH_ integer,intent(out) :: _N_
#  define _DECLARE_VERTICAL_ITERATORS_ integer :: _N_;_DECLARE_VERTICAL_INDICES_
#else
#  define _ARGUMENTS_VERTICAL_SLICE_LENGTH_
#  define _DECLARE_ARGUMENTS_VERTICAL_SLICE_LENGTH_
#  define _DECLARE_VERTICAL_ITERATORS_ _DECLARE_VERTICAL_INDICES_
#endif