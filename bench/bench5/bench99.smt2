(declare-const v0 Bool)
(declare-const v1 Bool)
(declare-const v2 Bool)
(declare-const v3 Bool)
(declare-const v4 Bool)
(assert (edge (not v0) (or v1 v2 v3)))
(assert (edge (or v1 v4) (or (not v1) (not v4))))
(assert (edge (or (not v1) (not v2) (not v4)) (or v2 v4)))
(assert (edge (not v4) v0))
(assert (edge true (or (not v0) (not v4))))
(assert (not (edge (or v2 (not v0) (not v4)) true)))