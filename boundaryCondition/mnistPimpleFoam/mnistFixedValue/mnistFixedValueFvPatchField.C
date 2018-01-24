/*---------------------------------------------------------------------------*\
  =========                 |
  \\      /  F ield         | OpenFOAM: The Open Source CFD Toolbox
   \\    /   O peration     |
    \\  /    A nd           | Copyright (C) 2011-2017 OpenFOAM Foundation
     \\/     M anipulation  | Copyright (C) 2017 OpenCFD Ltd.
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

\*---------------------------------------------------------------------------*/


#include "mnistFixedValueFvPatchField.H"

// * * * * * * * * * * * * * * * * Constructors  * * * * * * * * * * * * * * //

template<class Type>
Foam::mnistFixedValueFvPatchField<Type>::mnistFixedValueFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF
)
:
    mixedFvPatchField<Type>(p, iF),
    graphName_("dataFile"),
    imageName_("inputFile"),
    outputValue_(label(1))
{
    this->refValue() = Zero;
    this->refGrad() = Zero;
}


template<class Type>
Foam::mnistFixedValueFvPatchField<Type>::mnistFixedValueFvPatchField
(
    const mnistFixedValueFvPatchField<Type>& ptf,
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const fvPatchFieldMapper& mapper
)
:
    mixedFvPatchField<Type>(ptf, p, iF, mapper),
    graphName_(ptf.graphName_),
    imageName_(ptf.imageName_),
    outputValue_(ptf.outputValue_)
{}


template<class Type>
Foam::mnistFixedValueFvPatchField<Type>::mnistFixedValueFvPatchField
(
    const fvPatch& p,
    const DimensionedField<Type, volMesh>& iF,
    const dictionary& dict
)
:
    mixedFvPatchField<Type>(p, iF),
    graphName_(dict.lookupOrDefault<word>("dataFile", "dataFile")),
    imageName_(dict.lookupOrDefault<word>("inputFile", "inputFile")),
    outputValue_(dict.lookupOrDefault<label>("mnistOutput", label(1)))
{
    Field<Type> refValue_ = Field<Type>("refValue", dict, p.size());

    #include "mnist.H"

    this->refValue() = refValue_ * outputValue_;
    fvPatchField<Type>::operator=(this->refValue());
    this->refGrad() = Zero;
}


template<class Type>
Foam::mnistFixedValueFvPatchField<Type>::mnistFixedValueFvPatchField
(
    const mnistFixedValueFvPatchField<Type>& ptf
)
:
    mixedFvPatchField<Type>(ptf),
    graphName_(ptf.graphName_),
    imageName_(ptf.imageName_),
    outputValue_(ptf.outputValue_)
{}


template<class Type>
Foam::mnistFixedValueFvPatchField<Type>::mnistFixedValueFvPatchField
(
    const mnistFixedValueFvPatchField<Type>& ptf,
    const DimensionedField<Type, volMesh>& iF
)
:
    mixedFvPatchField<Type>(ptf, iF),
    graphName_(ptf.graphName_),
    imageName_(ptf.imageName_),
    outputValue_(ptf.outputValue_)
{}


// * * * * * * * * * * * * * * * Member Functions  * * * * * * * * * * * * * //

template<class Type>
void Foam::mnistFixedValueFvPatchField<Type>::updateCoeffs()
{
    if (this->updated())
    {
        return;
    }

    mixedFvPatchField<Type>::updateCoeffs();
}


template<class Type>
void Foam::mnistFixedValueFvPatchField<Type>::write(Ostream& os) const
{
    fvPatchField<Type>::write(os);
    os.writeEntryIfDifferent<word>("dataFile", "dataFile", graphName_);
    os.writeEntryIfDifferent<word>("inputFile", "inputFile", imageName_);
    this->refValue().writeEntry("refValue", os);
    os.writeEntryIfDifferent<label>("mnistOutput", label(1), outputValue_);
    this->writeEntry("value", os);
}

#include "mnistMenberFunctions.C"

// * * * * * * * * * * * * * * * Member Operators  * * * * * * * * * * * * * //


template<class Type>
void Foam::mnistFixedValueFvPatchField<Type>::operator=
(
    const fvPatchField<Type>& ptf
)
{
    fvPatchField<Type>::operator=
    (
        this->refValue()
    );
}


// ************************************************************************* //
