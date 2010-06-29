

// DhrystoneConstants

Ident_1     = 0;
Ident_2     = 1;
Ident_3     = 2;
Ident_4     = 3;
Ident_5     = 4;

public class Record_Type {
    Record_Type Record_Comp;
    int Discr;
    int Enum_Comp;
    int Int_Comp;
    String String_Comp;
    int Enum_Comp_2;
    String String_Comp_2;
    char Char_Comp_1;
    char Char_Comp_2;
}

Record_Type          Record_Glob, Next_Record_Glob;
int                  Int_Glob;
boolean              Bool_Glob;
char                 Char_Glob_1, Char_Glob_2;
int[]                Array_Glob_1    = new int[128];
int[][]              Array_Glob_2    = new int[128][128];
Record_Type          First_Record    = new Record_Type(),
Record_Type          Second_Record   = new Record_Type();

void Proc_1(Record_Type Pointer_Par_Val) {
	Record_Type Next_Record = Pointer_Par_Val.Record_Comp;
	Pointer_Par_Val.Record_Comp = Record_Glob;
	Pointer_Par_Val.Int_Comp = 5;
	Next_Record.Int_Comp = Pointer_Par_Val.Int_Comp;
	Next_Record.Record_Comp = Pointer_Par_Val.Record_Comp;
	Proc_3(Next_Record.Record_Comp);
	int[] Int_Ref = new int[1];
	if (Next_Record.Discr == Ident_1) {
		Next_Record.Int_Comp = 6;
		Int_Ref[0] = Next_Record.Enum_Comp;
		Proc_6(Pointer_Par_Val.Enum_Comp, Int_Ref);
		Next_Record.Enum_Comp = Int_Ref[0];
		Next_Record.Record_Comp = Record_Glob.Record_Comp;
		Int_Ref[0] = Next_Record.Int_Comp;
		Proc_7(Next_Record.Int_Comp, 10, Int_Ref);
		Next_Record.Int_Comp = Int_Ref[0];
		}else {
			Pointer_Par_Val = Pointer_Par_Val.Record_Comp;
		}
	}
}

void Proc_2(int Int_Par_Ref[]) {

        int Int_Loc;
        int Enum_Loc;

        Int_Loc = Int_Par_Ref[0] + 10;
        Enum_Loc = 0;

        do
            if (Char_Glob_1 == 'A') {
                Int_Loc -= 1;
                Int_Par_Ref[0] = Int_Loc - Int_Glob;
                Enum_Loc = Ident_1;
            }
        while (Enum_Loc != Ident_1);

    }

    void Proc_3(Record_Type Pointer_Par_Ref) {

        if (Record_Glob != null)
            Pointer_Par_Ref = Record_Glob.Record_Comp;
        else
            Int_Glob = 100;

        int[] Int_Comp_Ref = new int[1];
        Int_Comp_Ref[0] = Record_Glob.Int_Comp;
        Proc_7(10, Int_Glob, Int_Comp_Ref);
        Record_Glob.Int_Comp = Int_Comp_Ref[0];


    }

    void Proc_4() {
        boolean Bool_Loc;
        Bool_Loc = Char_Glob_1 == 'A';
        Bool_Loc = Bool_Loc || Bool_Glob;
        Char_Glob_2 = 'B';
    }

    void Proc_5() {

        Char_Glob_1 = 'A';
        Bool_Glob = false;

    }

    void Proc_6(int Enum_Par_Val, int Enum_Par_Ref[]) {

        Enum_Par_Ref[0] = Enum_Par_Val;

        if (!Func_3(Enum_Par_Val))
            Enum_Par_Ref[0] = Ident_4;

        switch (Enum_Par_Val) {

        case Ident_1:
           Enum_Par_Ref[0] = Ident_1;
            break;

        case Ident_2:
            if (Int_Glob > 100)
                Enum_Par_Ref[0] = Ident_1;
            else
                Enum_Par_Ref[0] = Ident_4;
            break;

        case Ident_3:
            Enum_Par_Ref[0] = Ident_2;
            break;

        case Ident_4:
            break;

        case Ident_5:
            Enum_Par_Ref[0] = Ident_3;
            break;
        }
    }