(declare-const v0 Bool)
(declare-const v1 Bool)
(declare-const v2 Bool)
(declare-const v3 Bool)
(declare-const v4 Bool)
(assert (edge true true))
(assert (edge (or v2 (not v3) (not v0)) (not v3)))
(assert (edge (not v0) (or v2 v3 v4)))
(assert (edge v0 (or (not v1) v2 (not v4))))
(assert (edge v1 (or v0 (not v3))))
(assert (not (edge (or v0 (not v1) (not v2)) true)))