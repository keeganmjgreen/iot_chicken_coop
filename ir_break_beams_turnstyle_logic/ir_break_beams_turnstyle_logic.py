while True:

    if A and B and not(C):
        AB_crossed = True

    if AB_crossed and A and not(B) and not(C):
        AB_crossed = False

    if AB_crossed and not(A) and B and not(C):
        A_passed = True
        AB_crossed = False

    if A_passed and A and not(B) and not(C):
        A_passed = False

    if A_passed and not(A) and B and C:
        BC_crossed = True

    if BC_crossed and not(A) and not(B) and C:
        B_passed = True
        BC_crossed = False

    if B_passed and not(A) and not(B) and not(C):
        C_passed = True
