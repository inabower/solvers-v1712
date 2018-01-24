/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2016 OpenFOAM Foundation
     \\/     M anipulation  |
-------------------------------------------------------------------------------
License
    This file is part of OpenFOAM.

    OpenFOAM is free software: you can redistribute it and/or modify it
    under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    OpenFOAM is distributed in the hope that it will be useful, but WITHOUT
    ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
    FITNESS FOR A PARTICULAR PURPOSE.  See the GNU General Public License
    for more details.

    You should have received a copy of the GNU General Public License
    along with OpenFOAM.  If not, see <http://www.gnu.org/licenses/>.

Class
    Foam::mnistFixedValueFvPatchField

Group
    grpOutletBoundaryConditions

Description
    This boundary condition provides a generic outflow condition, with
    specified inflow for the case of return flow.

Usage
    \table
        Property     | Description             | Required    | Default value
        phi          | Flux field name         | no          | phi
        inletValue   | Inlet value for reverse flow | yes    |
    \endtable

    Example of the boundary condition specification:
    \verbatim
    <patchName>
    {
        type            mnistFixedValue;
        phi             phi;
        inletValue      uniform 0;
        value           uniform 0;
    }
    \endverbatim

    The mode of operation is determined by the sign of the flux across the
    patch faces.

Note
    Sign conventions:
    - Positive flux (out of domain): apply zero-gradient condition
    - Negative flux (into of domain): apply the "inletValue" fixed-value

See also
    Foam::mixedFvPatchField
    Foam::zeroGradientFvPatchField
    Foam::outletInletFvPatchField

SourceFiles
    mnistFixedValueFvPatchField.C

\*---------------------------------------------------------------------------*/

#ifndef mnistFixedValueFvPatchField_H
#define mnistFixedValueFvPatchField_H

#include "mixedFvPatchField.H"

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

namespace Foam
{

/*---------------------------------------------------------------------------*\
                   Class mnistFixedValueFvPatchField Declaration
\*---------------------------------------------------------------------------*/

template<class Type>
class mnistFixedValueFvPatchField
:
    public mixedFvPatchField<Type>
{

protected:

    // Protected data

        //- Name of flux field
        word graphName_;
        word imageName_;
        label outputValue_;


public:

    //- Runtime type information
    TypeName("mnistFixedValue");


    // Constructors

        //- Construct from patch and internal field
        mnistFixedValueFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&
        );

        //- Construct from patch, internal field and dictionary
        mnistFixedValueFvPatchField
        (
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const dictionary&
        );

        //- Construct by mapping given mnistFixedValueFvPatchField onto a new patch
        mnistFixedValueFvPatchField
        (
            const mnistFixedValueFvPatchField<Type>&,
            const fvPatch&,
            const DimensionedField<Type, volMesh>&,
            const fvPatchFieldMapper&
        );

        //- Construct as copy
        mnistFixedValueFvPatchField
        (
            const mnistFixedValueFvPatchField<Type>&
        );

        //- Construct and return a clone
        virtual tmp<fvPatchField<Type>> clone() const
        {
            return tmp<fvPatchField<Type>>
            (
                new mnistFixedValueFvPatchField<Type>(*this)
            );
        }

        //- Construct as copy setting internal field reference
        mnistFixedValueFvPatchField
        (
            const mnistFixedValueFvPatchField<Type>&,
            const DimensionedField<Type, volMesh>&
        );

        //- Construct and return a clone setting internal field reference
        virtual tmp<fvPatchField<Type>> clone
        (
            const DimensionedField<Type, volMesh>& iF
        ) const
        {
            return tmp<fvPatchField<Type>>
            (
                new mnistFixedValueFvPatchField<Type>(*this, iF)
            );
        }


    // Member functions

        // Attributes

            //- Return true: this patch field is altered by assignment
            virtual bool assignable() const
            {
                return true;
            }


        //- Update the coefficients associated with the patch field
        virtual void updateCoeffs();

        //- Write
        virtual void write(Ostream&) const;

        #include "mnistMenberFunctions.H"


    // Member operators

        virtual void operator=(const fvPatchField<Type>& pvf);
};


// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

} // End namespace Foam

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#ifdef NoRepository
    #include "mnistFixedValueFvPatchField.C"
#endif

// * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * //

#endif

// ************************************************************************* //
