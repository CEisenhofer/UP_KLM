(declare-const v0 Bool)
(declare-const v1 Bool)
(declare-const v2 Bool)
(declare-const v3 Bool)
(declare-const v4 Bool)
(assert (edge (not v0) v1))
(assert (edge true (or v1 (not v0))))
(assert (edge (or v0 v3 (not v2)) (or v1 (not v4))))
(assert (edge (or v2 (not v3)) (not v0)))
(assert (edge (not v2) v4))
(assert (edge true true))
(assert (edge (or v2 (not v3)) (or v0 (not v2))))
(assert (edge (not v4) (not v2)))
(assert (edge (not v4) (or v1 v4 (not v2))))
(assert (edge (or (not v1) v4 (not v0)) (or v0 (not v1) (not v3))))
(assert (edge v4 (or v2 v4)))
(assert (edge (or v2 (not v4)) v2))
(assert (edge true (or v0 v1 v3)))
(assert (edge (or (not v1) v2) true))
(assert (edge (not v3) (or v1 (not v4))))
(assert (edge (or v0 (not v1)) (not v2)))
(assert (edge true (or v0 v3 (not v2))))
(assert (edge true (or v1 v3)))
(assert (edge (or v2 (not v3)) true))
(assert (edge (or v1 (not v3) (not v4)) (or v0 v2 (not v3))))
(assert (not (edge (or (not v1) (not v4)) (or v3 v4))))