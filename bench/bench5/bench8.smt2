(declare-const v0 Bool)
(declare-const v1 Bool)
(declare-const v2 Bool)
(declare-const v3 Bool)
(declare-const v4 Bool)
(assert (edge (or v1 v2 (not v0)) true))
(assert (edge (or v1 (not v2)) (or (not v3) (not v2))))
(assert (edge (or (not v1) v2 v3) (or v0 (not v3))))
(assert (edge (or v3 v4 (not v0)) (or v0 (not v3))))
(assert (edge (or v2 v4) v0))
(assert (not (edge (or (not v1) v4) (not v1))))